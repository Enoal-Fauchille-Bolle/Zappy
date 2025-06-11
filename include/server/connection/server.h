/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Structure Header
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include "client.h"
    #include "constants.h"
    #include "options_parser/options.h"

typedef struct server_s {
    int sockfd;
    unsigned int port;
    struct sockaddr_in addr;
    client_t *clients[MAX_CLIENTS];
} server_t;

server_t *create_server(server_options_t *options);

#endif /* !SERVER_H_ */
