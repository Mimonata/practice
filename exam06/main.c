/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 07:39:37 by spitul            #+#    #+#             */
/*   Updated: 2026/04/20 08:00:00 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
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

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

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

int	server_broadcast(int sender_fd, int max_fd, fd_set *read_fd, int server_fd, char *msg)
{
	for (int i = 0; i < max_fd; i++)
	{
		if (FD_ISSET(i, read_fd) && i != sender_fd && i != server_fd)
			send(i, msg, strlen(msg), 0);
	}	
}

int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli; 
	fd_set	read_fd;
	fd_set	work_fd;
	char	*client_buf[1024];
	int		client_ids[1024];
	int		maxfd;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(8081); 

    // Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully bound.\n");
	if (listen(sockfd, 10) != 0) {
		printf("cannot listen\n"); 
		exit(0); 
	}
    len = sizeof(cli);
	FD_ZERO(&read_fd);
	FD_SET(sockfd, &read_fd);
	bzero(client_ids, sizeof(client_ids));
	maxfd = sockfd;
	int next_id = 0;
	while(1)
	{
		work_fd = read_fd;
			
		if (select(maxfd + 1, &work_fd, NULL, NULL, NULL) <= 0)
			continue;
			
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if (FD_ISSET(fd, &work_fd))
			{
				if (fd == sockfd)
				{
					connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
					if (connfd < 0) { 
						printf("server accept failed...\n"); 
						exit(0); 
					} 
					else
						printf("server accept the client...\n");
					FD_SET(connfd, &read_fd);
					if (connfd > maxfd)
						maxfd = connfd;
					client_ids[connfd] = next_id++;
					client_buf[connfd] = NULL;
					char msg[100];
					sprintf(msg, "server: client %d just arrived\n", client_ids[connfd]);
					broadcast(connfd, maxfd, &read_fd, sockfd, msg);
				}
			}
		}
	}
	
}
