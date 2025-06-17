/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg_tests
*/

#include "connection/client.h"
#include "connection/server.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/tile.h"
#include "options_parser/options.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// void redirect_stdout(void)
// {
//     cr_redirect_stdout();
// }

Test(egg, create_egg, .timeout = 2)
{
    pos_t pos = {5, 4};
    void *team = NULL;      // Placeholder for team pointer
    egg_t *egg = create_egg(pos, team, false);

    cr_assert_not_null(egg, "Egg should not be NULL");
    cr_assert_eq(egg->pos.x, 5, "Egg X position should be 5");
    cr_assert_eq(egg->pos.y, 4, "Egg Y position should be 4");
    cr_assert_eq(egg->team, team, "Egg team pointer should match");
    destroy_egg(egg);
}

Test(egg, create_egg_null_team, .timeout = 2)
{
    pos_t pos = {5, 4};
    egg_t *egg = create_egg(pos, NULL, false);

    cr_assert_not_null(egg, "Egg should not be NULL even with NULL team");
    cr_assert_eq(egg->pos.x, 5, "Egg X position should be 5");
    cr_assert_eq(egg->pos.y, 4, "Egg Y position should be 4");
    cr_assert_null(egg->team, "Egg team pointer should be NULL");
    destroy_egg(egg);
}

Test(egg, destroy_egg_null, .timeout = 2)
{
    egg_t *egg = NULL;

    destroy_egg(egg);      // Should not crash
    cr_assert(true, "Destroying NULL egg should not cause a crash");
}

Test(egg, destroy_egg_valid, .timeout = 2)
{
    pos_t pos = {5, 4};
    void *team = NULL;      // Placeholder for team pointer
    egg_t *egg = create_egg(pos, team, false);

    cr_assert_not_null(egg, "Egg should not be NULL");
    destroy_egg(egg);      // Should not crash
    cr_assert(true, "Destroying valid egg should not cause a crash");
}

Test(egg, add_egg_to_tile_null, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_tile(NULL, egg);
    add_egg_to_tile(tile, NULL);
    add_egg_to_tile(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
}

Test(egg, remove_egg_from_tile_null, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    remove_egg_from_tile(NULL, egg);
    remove_egg_from_tile(tile, NULL);
    remove_egg_from_tile(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
}

Test(egg, add_egg_to_tile_valid, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    cr_assert_not_null(tile->eggs, "Tile eggs vector should not be NULL");
    add_egg_to_tile(tile, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Tile should have one egg after adding");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_tile_valid, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    egg_t *another_egg = create_egg((pos_t){1, 1}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_tile(tile, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Tile should have one egg before removal");
    remove_egg_from_tile(tile, egg);
    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Tile should have no eggs after removal");
    add_egg_to_tile(tile, another_egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        another_egg, "Tile should have another egg after adding another egg");
    // No assertion here, just checking for crashes
    remove_egg_from_tile(tile, egg);      // Should not crash
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_egg(another_egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_tile_not_found, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    remove_egg_from_tile(tile, egg);      // Should not crash
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, add_egg_to_map_null, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);

    add_egg_to_map(NULL, egg);
    add_egg_to_map(map, NULL);
    add_egg_to_map(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_map_null, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);

    remove_egg_from_map(NULL, egg);
    remove_egg_from_map(map, NULL);
    remove_egg_from_map(NULL, NULL);
    // No assertion here, just checking for crashes
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, add_egg_to_map_valid, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Tile eggs vector should be empty before adding");
    add_egg_to_map(map, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Map tile should have one egg after adding");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, remove_egg_from_map_valid, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);
    map_t *map = create_map(1, 1, false);
    tile_t *tile = get_tile(map, (pos_t){0, 0});

    add_egg_to_map(map, egg);
    cr_assert_eq(*(egg_t **)vector_get_vtable(tile->eggs)->at(tile->eggs, 0),
        egg, "Map tile should have one egg before removal");
    remove_egg_from_map(map, egg);
    cr_assert_eq(vector_get_vtable(tile->eggs)->size(tile->eggs), 0,
        "Map tile should have no eggs after removal");
    destroy_egg(egg);
    destroy_map(map);
}

Test(egg, spawn_player_from_egg_null, .timeout = 2)
{
    map_t *map = create_map(1, 1, false);
    player_t *player = spawn_player_from_egg(NULL, map, 1, NULL);
    egg_t *egg = create_egg((pos_t){0, 0}, NULL, false);

    cr_assert_null(player, "Player should be NULL when egg is NULL");
    player = spawn_player_from_egg(NULL, NULL, 1, NULL);
    cr_assert_null(
        player, "Player should be NULL when both egg and map are NULL");
    player = spawn_player_from_egg(egg, NULL, 1, NULL);
    cr_assert_null(
        player, "Player should be NULL when egg is NULL but map is valid");
    destroy_map(map);
}

Test(egg, spawn_player_from_egg_valid, .timeout = 2)
{
    egg_t *egg = create_egg((pos_t){7, 4}, NULL, false);
    map_t *map = create_map(10, 10, false);
    player_t *player;
    tile_t *tile = get_tile(map, (pos_t){7, 4});

    cr_assert_eq(vector_get_vtable(tile->players)->size(tile->players), 0,
        "Tile should have zero players");
    player = spawn_player_from_egg(egg, map, 1, NULL);
    cr_assert_not_null(player, "Player should not be NULL after spawning");
    cr_assert_eq(player->pos.x, 7, "Player X position should match egg");
    cr_assert_eq(player->pos.y, 4, "Player Y position should match egg");
    cr_assert_eq(vector_get_vtable(tile->players)->size(tile->players), 1,
        "Tile should have one player after spawning from egg");

    destroy_player(player);
    destroy_map(map);
}

Test(egg, spawn_minimum_eggs_null, .timeout = 2)
{
    map_t *map = create_map(1, 1, false);
    team_t *team = create_team("TestTeam");

    spawn_min_eggs(NULL, team, 5, false);      // Should not crash
    spawn_min_eggs(map, NULL, 5, false);       // Should not crash
    spawn_min_eggs(NULL, NULL, 5, false);      // Should not crash

    destroy_team(team);
    destroy_map(map);
}

Test(egg, spawn_minimum_eggs_valid, .timeout = 2)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");

    spawn_min_eggs(map, team, 5, false);
    cr_assert_eq(
        get_egg_count(team), 5, "Team should have 5 eggs after spawning");

    for (size_t i = 0; i < get_egg_count(team); i++) {
        egg_t *egg =
            *(egg_t **)vector_get_vtable(team->eggs)->at(team->eggs, i);
        cr_assert_not_null(egg, "Egg should not be NULL");
        cr_assert_eq(egg->team, team, "Egg team should match the team");
        cr_assert(vector_get_vtable(get_tile(map, egg->pos)->eggs)
                          ->size(get_tile(map, egg->pos)->eggs) > 0,
            "Tile should have at least one egg after spawning");
    }
    destroy_team(team);
    destroy_map(map);
}

Test(egg, spawn_minimum_eggs_twice, .timeout = 2)
{
    map_t *map = create_map(10, 10, false);
    team_t *team = create_team("TestTeam");

    spawn_min_eggs(map, team, 2, false);
    cr_assert_eq(
        get_egg_count(team), 2, "Team should have 2 eggs after spawning");
    spawn_min_eggs(map, team, 30, false);
    cr_assert_eq(
        get_egg_count(team), 30, "Team should have 30 eggs after spawning");

    for (size_t i = 0; i < get_egg_count(team); i++) {
        egg_t *egg =
            *(egg_t **)vector_get_vtable(team->eggs)->at(team->eggs, i);
        cr_assert_not_null(egg, "Egg should not be NULL");
        cr_assert_eq(egg->team, team, "Egg team should match the team");
        cr_assert(vector_get_vtable(get_tile(map, egg->pos)->eggs)
                          ->size(get_tile(map, egg->pos)->eggs) > 0,
            "Tile should have at least one egg after spawning");
    }
    destroy_team(team);
    destroy_map(map);
}

Test(egg, player_lays_egg_null, .timeout = 2)
{
    team_t *team = create_team("TestTeam");
    player_t *player = create_player((pos_t){0, 0}, 1, team, NULL);
    map_t *map = create_map(1, 1, false);

    lay_egg(NULL, map);         // Should not crash
    lay_egg(player, NULL);      // Should not crash
    lay_egg(NULL, NULL);        // Should not crash
    player->team = NULL;        // Set team to NULL to avoid memory leak
    lay_egg(player, map);       // Should not crash

    destroy_team(team);
    destroy_map(map);
}

Test(egg, player_lays_egg_valid, .timeout = 2)
{
    char **teams = malloc(sizeof(char *) * 2);
    teams[0] = malloc(sizeof(char) * 9);
    strcpy(teams[0], "TeamTest");
    teams[1] = NULL;
    server_options_t *options = malloc(sizeof(server_options_t));
    options->port = 4242;
    options->width = 10;
    options->height = 10;
    options->debug = true;
    options->clients_nb = 1;
    options->error = false;
    options->frequency = DEFAULT_TICK_RATE;
    options->help = false;
    options->teams = teams;
    server_t *server = create_server(options);
    server->clients[0] = create_client(server, server->game->teams[0], 0);
    client_t *client = server->clients[0];
    player_t *player = client->player;
    egg_t *egg;

    cr_assert_eq(player->tick_cooldown, 0,
        "Player should have no cooldown before laying an egg");
    cr_assert_eq(get_egg_count(server->game->teams[0]), 0, "Team should have no eggs initially");
    egg = lay_egg(player, server->game->map);
    cr_assert_not_null(egg, "Egg should not be NULL after laying");
    cr_assert_eq(egg->pos.x, player->pos.x,
        "Egg X position should match player position");
    cr_assert_eq(egg->pos.y, player->pos.y,
        "Egg Y position should match player position");
    cr_assert_eq(egg->team, server->game->teams[0], "Egg team should match the player's team");
    cr_assert_eq(
        get_egg_count(server->game->teams[0]), 1, "Team should have one egg after laying");
    cr_assert_eq(player->tick_cooldown, 42,
        "Player should have cooldown of 42 ticks after laying an egg");
    destroy_server(server);
}
