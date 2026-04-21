#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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

int	error()
{
	write(2, "Fatal error\n", 12);
	exit (1);
}

int main(int argc, char **argv) 
{
	int sockfd, connfd;
	struct sockaddr_in servaddr; 

	if (argc != 2)
	{
		write(2, "Wrong arguments\n", 16);
		exit (1);
	}
	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) 
		error();
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
		error();
	if (listen(sockfd, 10) != 0) 
		error();

	fd_set	read, active;
	int	id[1064];
	char	*msgs[1064];
	int	fd;
	int	nextid = 0;
	int	maxfd = sockfd;
	unsigned int	len = sizeof(servaddr);
	char msg[64];

	FD_ZERO(&read);
	FD_SET(sockfd, &read);
	bzero(msgs, sizeof(msgs));

	while (1)
	{
		active = read;
		if (select(maxfd + 1, &active, 0, 0, 0) == 0)
			continue;
		for (fd = 0; fd <= maxfd; fd++)
		{
			if (FD_ISSET(fd, &active))
			{
				if (fd == sockfd)
				{
					if ((connfd = accept(sockfd, (struct sockaddr *)&servaddr, &len)) < 0)
						error();
					FD_SET(connfd, &read);
					id[connfd] = nextid;
					nextid++;
					if (connfd > maxfd)
						maxfd = connfd;

					sprintf(msg, "Client %d: has joined\n", id[connfd]);
					for (int i = 0; i <= maxfd; i++)
					{
						if (FD_ISSET(i, &read) && connfd != i && sockfd != i)
							send(i, msg, sizeof(msg), 0);
					}
					bzero(msg, sizeof(msg));
				}
				else
				{
					char	buffer[1024];
					int	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
					buffer[bytes] = '\0';
					if (bytes == 0)
					{
						sprintf(msg, "Client %d: has left\n", id[fd]);						
						for (int i = 0; i <= maxfd; i++)
						{
							if (FD_ISSET(i, &read) && connfd != i && sockfd != i)
								send(i, msg, sizeof(msg), 0);
						}
						bzero(msg, sizeof(msg));
						FD_CLR(fd, &read);
						close (fd);
						free(msgs[fd]);
					}
					else
					{
						msgs[fd] = str_join(msgs[fd], buffer);
						if (!msgs[fd])
							error();
						int mes_ext = -1;
						char	*mess = NULL;
						char	prefix[64];
						sprintf(prefix, "Client %d:", id[fd]);
						while ((mes_ext = extract_message(&msgs[fd], &mess)) == 1)
						{
							for(int i = 0; i <= maxfd; i++)
							{
								if (FD_ISSET(i, &read) && i != sockfd && i != fd)
								{
									send(i, prefix, strlen(prefix), 0);
									send(i, mess, strlen(mess), 0);
								}
							}
							free(mess);
						}
						if (mes_ext == -1) error();
					}
				}
			}
		}
	}
	return 0;
}
