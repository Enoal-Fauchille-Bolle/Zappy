/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Fork Command Tests
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include "map/coordinates.h"
#include "options_parser/options.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "game/game.h"
#include "team/player/player.h"
#include "map/map.h"
#include "game/game_constants.h"
#include "command_handler/ai_commands.h"

static client_t *create_test_client(int x, int y, orientation_t orientation)
{
    client_t *client = calloc(1, sizeof(client_t));
    server_t *server = calloc(1, sizeof(server_t));
    game_t *game = calloc(1, sizeof(game_t));
    map_t *map = create_map(5, 5, false);
    player_t *player = create_player((pos_t){x, y}, 1, NULL, NULL);
    player->orientation = orientation;
    client->player = player;
    client->server = server;
    server->game = game;
    game->map = map;
    server->options = calloc(1, sizeof(server_options_t));
    return client;
}

static void destroy_test_client(client_t *client)
{
    if (!client) return;
    if (client->player) destroy_player(client->player);
    if (client->server) {
        if (client->server->game) {
            if (client->server->game->map)
                destroy_map(client->server->game->map);
            free(client->server->game);
        }
        if (client->server->options)
            free(client->server->options);
        free(client->server);
    }
    free(client);
}

Test(fork_command, fork_basic, .timeout = 2) {
    client_t *client = create_test_client(2, 2, NORTH);
    command_t cmd = {0};
    fork_command(client, &cmd);
    // No assert: just check it doesn't crash for now
    destroy_test_client(client);
}
