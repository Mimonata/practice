/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniserv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 07:39:27 by spitul            #+#    #+#             */
/*   Updated: 2026/03/26 07:11:28 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <stdio.h>
#include <sys/select.h>
#include <netinet/in.h>

fd_set	active_fd;
fd_set	read_fd;
int		max_fd;
char	*msg[1024];

int	*setup_server(int port)
{
	int	sfd;
	struct sockaddr_in	servaddr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(port);
	bind(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(sfd, 10);
	return &sfd;
}

void	minserv(int port)
{
	int		client_ids[1024];
	char	*client_msg[1024];
	fd_set	working_fd, ref_fd;
	int		sockfd;
	
	
}
