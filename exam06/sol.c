#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>

// -----------------------------------------------------------------------------
// PROVIDED HELPER FUNCTIONS
// -----------------------------------------------------------------------------

int extract_message(char **buf, char **msg) {
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i]) {
        if ((*buf)[i] == '\n') {
            newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                return (-1);
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return (1);
        }
        i++;
    }
    return (0);
}

char *str_join(char *buf, char *add) {
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0)
        return (0);
    newbuf[0] = 0;
    if (buf != 0)
        strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

// -----------------------------------------------------------------------------
// SERVER LOGIC
// -----------------------------------------------------------------------------

void fatal_error() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    // 1. Socket setup
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) fatal_error();

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
    servaddr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) fatal_error();
    if (listen(sockfd, 10) != 0) fatal_error();

    // 2. Select setup
    fd_set active_fds, read_fds;
    FD_ZERO(&active_fds);
    FD_SET(sockfd, &active_fds);
    int max_fd = sockfd;

    // 3. Client tracking variables
    int next_id = 0;
    int ids[65536];         // Maps an FD to a client ID
    char *msgs[65536];      // Stores partial messages for each FD
    bzero(msgs, sizeof(msgs));

    // 4. Main Event Loop
    while (1) {
        read_fds = active_fds;
        
        // Block until something happens on one of our tracked FDs
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) continue;

        for (int fd = 0; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &read_fds)) {
                
                // EVENT A: New client is connecting
                if (fd == sockfd) {
                    int connfd = accept(sockfd, NULL, NULL);
                    if (connfd < 0) fatal_error();
                    
                    FD_SET(connfd, &active_fds);
                    if (connfd > max_fd) max_fd = connfd;
                    
                    ids[connfd] = next_id++;
                    msgs[connfd] = NULL;

                    char buf[64];
                    sprintf(buf, "server: client %d just arrived\n", ids[connfd]);
                    
                    // Broadcast arrival
                    for (int i = 0; i <= max_fd; i++) {
                        if (FD_ISSET(i, &active_fds) && i != sockfd && i != connfd) {
                            send(i, buf, strlen(buf), 0);
                        }
                    }
                } 
                // EVENT B: Existing client is sending data or disconnecting
                else {
                    char buffer[4096];
                    int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
                    
                    // Disconnect
                    if (bytes <= 0) {
                        char buf[64];
                        sprintf(buf, "server: client %d just left\n", ids[fd]);
                        
                        // Broadcast departure
                        for (int i = 0; i <= max_fd; i++) {
                            if (FD_ISSET(i, &active_fds) && i != sockfd && i != fd) {
                                send(i, buf, strlen(buf), 0);
                            }
                        }
                        close(fd);
                        free(msgs[fd]);
                        msgs[fd] = NULL;
                        FD_CLR(fd, &active_fds);
                    } 
                    // Data Received
                    else {
                        buffer[bytes] = '\0';
                        msgs[fd] = str_join(msgs[fd], buffer);
                        if (!msgs[fd]) fatal_error(); // Malloc failed in str_join

                        char *msg = NULL;
                        int ext_res;
                        
                        // Extract lines ending with '\n'
                        while ((ext_res = extract_message(&msgs[fd], &msg)) == 1) {
                            char prefix[64];
                            sprintf(prefix, "client %d: ", ids[fd]);
                            
                            // Broadcast message with prefix
                            for (int i = 0; i <= max_fd; i++) {
                                if (FD_ISSET(i, &active_fds) && i != sockfd && i != fd) {
                                    send(i, prefix, strlen(prefix), 0);
                                    send(i, msg, strlen(msg), 0);
                                }
                            }
                            free(msg); // extract_message hands over allocated memory
                        }
                        if (ext_res == -1) fatal_error(); // Calloc failed in extract_message
                    }
                }
            }
        }
    }
    return 0;
}
