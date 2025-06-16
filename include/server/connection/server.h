/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Structure Header
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <sys/poll.h>
    #include "constants.h"
    #include "connection/client.h"
    #include "options_parser/options.h"
    #include "game/game.h"

typedef struct server_s {
    server_options_t *options;
    struct pollfd fds[MAX_CLIENTS + 2]; // +2 for server socket and signal fd
    // char *clients_team[MAX_CLIENTS];
    client_t *clients[MAX_CLIENTS];
    game_t *game;
} server_t;

server_t *create_server(server_options_t *options);
void destroy_server(server_t *server);
void run_server(server_t *server);

#endif /* !SERVER_H_ */
