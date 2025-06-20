/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** spawn
*/

#include "connection/client.h"
#include "connection/server.h"
#include "command_handler/gui_commands.h"
#include "constants.h"
#include "debug_categories.h"
#include "map/coordinates.h"
#include "map/orientation_names.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Spawn a minimum number of eggs on the map for a given team.
 *
 * This function generates a specified number of eggs at random positions
 * within the map's dimensions. Each egg is associated with a team pointer.
 *
 * @param map Pointer to the map structure where eggs will be spawned
 * @param team Pointer to the team structure for which eggs will be spawned
 * @param count Number of eggs to spawn
 */
void spawn_min_eggs(map_t *map, team_t *team, size_t min, bool debug)
{
    pos_t random_pos;
    egg_t *current_egg;

    if (map == NULL || team == NULL) {
        fprintf(stderr, "Invalid map or team pointer\n");
        return;
    }
    for (size_t i = 0; i < min; i++) {
        if (get_egg_count(team) >= (min - get_team_player_count(team)))
            break;
        random_pos = (pos_t){rand() % map->width, rand() % map->height};
        current_egg = create_egg(random_pos, team, 0, debug);
        if (current_egg == NULL) {
            fprintf(stderr, "Failed to create egg\n");
            continue;
        }
        add_egg_to_map(map, current_egg);
    }
}

/**
 * @brief Check if the egg pointer is valid.
 *
 * This function checks if the egg pointer is not NULL. If it is NULL,
 * it prints an error message and returns FAILURE.
 *
 * @param egg Pointer to the egg structure to check
 * @return SUCCESS if the egg pointer is valid, FAILURE otherwise
 */
static bool check_egg_validity(egg_t *egg)
{
    if (egg == NULL) {
        fprintf(stderr, "Invalid egg\n");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Check if the map pointer is valid.
 *
 * This function checks if the map pointer is not NULL. If it is NULL,
 * it prints an error message and returns FAILURE.
 *
 * @param map Pointer to the map structure to check
 * @return SUCCESS if the map pointer is valid, FAILURE otherwise
 */
static bool check_map_validity(map_t *map)
{
    if (map == NULL) {
        fprintf(stderr, "Map pointer is NULL\n");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Check if the player pointer is valid.
 *
 * This function checks if the player pointer is not NULL. If it is NULL,
 * it prints an error message and returns FAILURE.
 *
 * @param player Pointer to the player structure to check
 * @return SUCCESS if the player pointer is valid, FAILURE otherwise
 */
static bool check_player_validity(player_t *player)
{
    if (player == NULL) {
        fprintf(stderr, "Player pointer is NULL\n");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Debug function to log player spawn information.
 *
 * This function logs the player's spawn position and orientation,
 * as well as the egg removal from the team.
 *
 * @param client Pointer to the client structure for logging
 * @param player Pointer to the player structure being spawned
 * @param egg Pointer to the egg structure being removed
 */
static void debug_player_spawn(client_t *client, player_t *player,
    egg_t *egg)
{
    debug_map(client->server->options->debug,
        "Player %zu spawned at position (%d, %d) with orientation %s\n",
        player->id, player->pos.x, player->pos.y,
        orientation_names[player->orientation]);
    debug_map(client->server->options->debug,
        "Egg %zu removed from team '%s'\n", egg->id, egg->team->name);
}

/**
 * @brief Spawn a player from an egg and add it to the map.
 *
 * This function creates a new player at the position of the given egg,
 * adds the player to the map, and removes the egg from the map.
 * It returns a pointer to the newly created player.
 *
 * @param egg Pointer to the egg structure from which to spawn the player
 * @param map Pointer to the map structure where the player will be added
 * @return Pointer to the newly created player_t structure on success,
 *         NULL if egg or map is NULL or if player creation fails
 */
player_t *spawn_player_from_egg(
    egg_t *egg, map_t *map, const size_t player_id, client_t *client)
{
    player_t *player;

    if (check_egg_validity(egg) == FAILURE ||
        check_map_validity(map) == FAILURE)
        return NULL;
    player = create_player(egg->pos, player_id, egg->team, client);
    if (check_player_validity(player) == FAILURE)
        return NULL;
    add_player_to_map(map, player);
    if (client != NULL && client->server != NULL &&
        client->server->options != NULL) {
        debug_player_spawn(client, player, egg);
    }
    ebo_command(egg);
    remove_egg_from_map(map, egg);
    remove_egg_from_team(egg->team, egg);
    destroy_egg(egg);
    return player;
}
