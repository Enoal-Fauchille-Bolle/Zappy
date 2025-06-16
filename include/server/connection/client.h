/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include "team/player/player.h"
    #include "team/team.h"

typedef struct server_s server_t;

typedef struct client_s {
    char *team_name;
    player_t *player;
} client_t;

void handle_client_message(server_t *server, int client_index);
bool handle_team_join(
    server_t *server, const char *team_name, int client_index);
client_t *create_client(server_t *server, team_t *team);
void destroy_client(client_t *client);

#endif /* !CLIENT_H_ */
