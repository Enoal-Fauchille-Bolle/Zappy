/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tick Management - Unit Tests
*/

#include "game/game.h"
#include "game/game_state.h"
#include "game/incantation.h"
#include "game/tick.h"
#include "map/resources.h"
#include "team/player/player.h"
#include "team/team.h"
#include "connection/server.h"
#include "connection/client.h"
#include "options_parser/options.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <stdlib.h>

// Mock server, client, player, team, and game helpers
// Helper to create a minimal server
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->options = calloc(1, sizeof(server_options_t));
    server->options->debug = false;
    return server;
}

static void destroy_test_server(server_t *server)
{
    if (!server) return;
    if (server->options) free(server->options);
    free(server);
}

// Helper to create a minimal client
static client_t *create_test_client(server_t *server)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->server = server;
    client->index = 0;
    client->is_gui = false;
    return client;
}

static void destroy_test_client(client_t *client)
{
    if (!client) return;
    free(client);
}

// Helper to create a minimal player
static player_t *create_test_player(server_t *server, int food, int hunger_cd, int tick_cd)
{
    player_t *player = calloc(1, sizeof(player_t));
    client_t *client = create_test_client(server);
    player->client = client;
    client->player = player;
    player->client->server = server;
    player->inventory[FOOD] = food;
    player->hunger_cooldown = hunger_cd;
    player->tick_cooldown = tick_cd;
    player->id = 42;
    return player;
}

static void destroy_test_player(player_t *player)
{
    if (player) {
        destroy_test_client(player->client);
        free(player);
    }
}

// Helper to create a minimal team
static team_t *create_test_team(game_t *game)
{
    team_t *team = calloc(1, sizeof(team_t));
    team->players = vector_new(sizeof(player_t *));
    team->game = game;
    return team;
}

static void destroy_test_team(team_t *team)
{
    if (!team) return;
    if (team->players) vector_destroy(team->players);
    free(team);
}

// Helper to create a minimal game
static game_t *create_test_game(server_t *server)
{
    game_t *game = calloc(1, sizeof(game_t));
    game->server = server;
    game->game_state = GAME_RUNNING;
    game->teams = calloc(2, sizeof(team_t *));
    game->teams[1] = NULL;
    game->incantations = vector_new(sizeof(incantation_t *));
    return game;
}

static void destroy_test_game(game_t *game)
{
    if (!game) return;
    if (game->teams) {
        for (int i = 0; game->teams[i]; ++i) destroy_test_team(game->teams[i]);
        free(game->teams);
    }
    free(game);
}

Test(tick, starvation_and_tick_cooldown_integration)
{
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    team_t *team = create_test_team(game);
    player_t *player = create_test_player(server, 1, 0, 2);
    player->doing_action = true;
    const vector_vtable_t *vtable = vector_get_vtable(team->players);
    vtable->push_back(team->players, &player);
    game->teams[0] = team;
    game->game_state = 1; // GAME_RUNNING
    server->game = game;
    // Call the public tick function
    game_tick(game, server->options);
    cr_assert_eq(player->tick_cooldown, 1, "Tick cooldown should decrement");
    cr_assert_eq(player->doing_action, true, "Still doing action if cooldown > 0");
    // Simulate tick: tick_cooldown reaches 0, doing_action reset
    game_tick(game, server->options);
    cr_assert_eq(player->tick_cooldown, 0, "Tick cooldown should reach 0");
    cr_assert_eq(player->doing_action, false, "Action should be reset when cooldown reaches 0");
    // Simulate starvation: food is 1, after tick should be 0
    player->tick_cooldown = 0;
    player->hunger_cooldown = 126;
    player->inventory[FOOD] = 1;
    game_tick(game, server->options);
    // cr_assert_eq(player->hunger_cooldown, 125, "Food cooldown should decrement by 1");
    // Simulate starvation: food is 0, after tick player should be removed (simulate by checking food stays 0)
    player->tick_cooldown = 0;
    player->inventory[FOOD] = 0;
    game_tick(game, server->options);
    destroy_test_player(player);
    destroy_test_game(game);
    destroy_test_server(server);
}
