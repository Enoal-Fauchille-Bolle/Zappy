/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Socket Header
*/

#ifndef SOCKET_H_
    #define SOCKET_H_

    #include "options_parser/options.h"
    #include "connection/server.h"

bool setup_socket(server_t *server, int port);
char *read_socket(int sockfd);

#endif /* !SOCKET_H_ */
