/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniserv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 07:39:27 by spitul            #+#    #+#             */
/*   Updated: 2026/03/11 07:45:00 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <stdio.h>
#include <sys/select.h>

void	minserv(int port)
{
	int		client_ids[1024];
	char	*client_msg[1024];
	fd_set	working_fd, ref_fd;
	int		sockfd;
	
	
}
