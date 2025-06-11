/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <netinet/in.h>
    #include <stdio.h>
    #include <poll.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdbool.h>

void process_client_message(struct pollfd *fds, int client_index, bool debug);

#endif /* !CLIENT_H_ */
