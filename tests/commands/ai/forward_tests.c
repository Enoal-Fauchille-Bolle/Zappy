/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Forward Command Tests
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

Test(forward_command, move_north, .timeout = 2) {
    client_t *client = create_test_client(2, 2, NORTH);
    command_t cmd = {0};
    forward_command(client, &cmd);
    cr_assert_eq(client->player->pos.x, 2);
    cr_assert_eq(client->player->pos.y, 1);
    cr_assert_eq(client->player->tick_cooldown, FORWARD_COMMAND_COOLDOWN);
    destroy_test_client(client);
}

Test(forward_command, move_east, .timeout = 2) {
    client_t *client = create_test_client(2, 2, EAST);
    command_t cmd = {0};
    forward_command(client, &cmd);
    cr_assert_eq(client->player->pos.x, 3);
    cr_assert_eq(client->player->pos.y, 2);
    destroy_test_client(client);
}

Test(forward_command, move_south, .timeout = 2) {
    client_t *client = create_test_client(2, 2, SOUTH);
    command_t cmd = {0};
    forward_command(client, &cmd);
    cr_assert_eq(client->player->pos.x, 2);
    cr_assert_eq(client->player->pos.y, 3);
    destroy_test_client(client);
}

Test(forward_command, move_west, .timeout = 2) {
    client_t *client = create_test_client(2, 2, WEST);
    command_t cmd = {0};
    forward_command(client, &cmd);
    cr_assert_eq(client->player->pos.x, 1);
    cr_assert_eq(client->player->pos.y, 2);
    destroy_test_client(client);
}

Test(forward_command, wrap_around_map, .timeout = 2) {
    client_t *client = create_test_client(0, 0, NORTH);
    command_t cmd = {0};
    forward_command(client, &cmd);
    cr_assert_eq(client->player->pos.x, 0);
    cr_assert_eq(client->player->pos.y, 4); // Should wrap around
    destroy_test_client(client);
}

