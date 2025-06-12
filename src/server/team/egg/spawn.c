/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** spawn
*/

#include "team/egg/egg.h"
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
// TODO: Add each spawned egg to the team's egg list when implemented. Check in
// the team structure that the egg count of the team does not exceed the
// maximum count parameter
void spawn_min_eggs(map_t *map, void *team, size_t min)
{
    pos_t random_pos;
    egg_t *current_egg;

    if (map == NULL || team == NULL) {
        fprintf(stderr, "Invalid map or team pointer\n");
        return;
    }
    for (size_t i = 0; i < min; i++) {
        random_pos = (pos_t){rand() % map->width, rand() % map->height};
        current_egg = create_egg(random_pos, team);
        if (current_egg == NULL) {
            fprintf(stderr, "Failed to create egg\n");
            continue;
        }
        add_egg_to_map(map, current_egg);
    }
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
// TODO: Add new player to the egg's team player list when implemented and
// remove the egg from the team's egg list
player_t *spawn_player_from_egg(egg_t *egg, map_t *map)
{
    player_t *player;

    if (egg == NULL || map == NULL) {
        fprintf(stderr, "Invalid egg or map pointer\n");
        return NULL;
    }
    player = create_player(egg->pos);
    if (player == NULL) {
        fprintf(stderr, "Failed to create player from egg\n");
        return NULL;
    }
    add_player_to_map(map, player);
    remove_egg_from_map(map, egg);
    return player;
}
