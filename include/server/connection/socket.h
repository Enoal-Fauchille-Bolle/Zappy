/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Socket Header
*/

#ifndef SOCKET_H_
    #define SOCKET_H_

    #include <stdbool.h>
    #include "connection/server.h"

int setup_socket_fd(void);
bool setup_socket(server_t *server, int port);
char *read_socket(int sockfd);

#endif /* !SOCKET_H_ */
