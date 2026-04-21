for (fd = 0; fd <= maxfd; fd++) // Fix 2: Changed to <=
{
    if (FD_ISSET(fd, &active))
    {
        if (fd == sockfd) // Fix 1: Check fd, not connfd
        {
            if ((connfd = accept(sockfd, (struct sockaddr *)&servaddr, &len)) < 0)
                error();
            FD_SET(connfd, &read);
            id[connfd] = nextid++;
            if (connfd > maxfd)
                maxfd = connfd;

            sprintf(msg, "Client %d: has joined\n", id[connfd]);
            for (int i = 0; i <= maxfd; i++) // Fix 3: Use 'i' instead of 'fd'
            {
                if (FD_ISSET(i, &read) && connfd != i && sockfd != i)
                    send(i, msg, strlen(msg), 0);
            }
        }
        else
        {
            char buffer[1024];
            int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0)
            {
                sprintf(msg, "Client %d: has left\n", id[fd]);						
                for (int i = 0; i <= maxfd; i++) // Fix 3: Use 'i' instead of 'fd'
                {
                    if (FD_ISSET(i, &read) && fd != i && sockfd != i)
                        send(i, msg, strlen(msg), 0);
                }
                FD_CLR(fd, &read);
                close(fd);
                free(msgs[fd]);
                msgs[fd] = NULL;
            }
            else
            {
                buffer[bytes] = '\0'; // Fix 4: Correct null termination
                msgs[fd] = str_join(msgs[fd], buffer);
                if (!msgs[fd])
                    error();
                
                char *mess = NULL;
                char prefix[64]; // Fix 4: Pre-allocate array
                sprintf(prefix, "Client %d: ", id[fd]);
                
                while (extract_message(&msgs[fd], &mess) == 1)
                {
                    for(int i = 0; i <= maxfd; i++)
                    {
                        if (FD_ISSET(i, &read) && i != sockfd && i != fd)
                        {
                            send(i, prefix, strlen(prefix), 0); // Fix 5: strlen, no '&'
                            send(i, mess, strlen(mess), 0);
                        }
                    }
                    free(mess); // Fix 6: Free inside the loop
                }
            }
        }
    }
}
