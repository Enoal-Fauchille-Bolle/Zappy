/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include "command_handler/command.h"
    #include "constants.h"
    #include <stdbool.h>

typedef struct server_s server_t;
typedef struct team_s team_t;
typedef struct player_s player_t;

typedef struct client_s {
    server_t *server;
    int index;
    int sockfd;
    player_t *player;
    command_t *command[MAX_COMMAND_BUFFER_SIZE];    // Queue for commands
} client_t;

void handle_client_message(server_t *server, int client_index);
bool handle_team_join(
    server_t *server, const char *team_name, int client_index);
client_t *create_client(server_t *server, team_t *team, int client_index);
void destroy_client(client_t *client);

#endif /* !CLIENT_H_ */
