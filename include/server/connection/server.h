/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Structure Header
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdbool.h>
    #include <sys/poll.h>
    #include "constants.h"
    #include "connection/client.h"
    #include "options_parser/options.h"

typedef struct server_s {
    int sockfd;
    unsigned int port;
    server_options_t *options;
    struct pollfd fds[MAX_CLIENTS + 1];
    char **client_teams;
} server_t;

server_t *create_server(server_options_t *options);
void destroy_server(server_t *server);
void run_server(server_t *server);

#endif /* !SERVER_H_ */
