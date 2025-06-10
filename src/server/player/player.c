/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "player/player.h"
#include "map/coordinates.h"
#include "map/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

player_t *create_player(size_t id, pos_t pos, size_t team_id)
{
    player_t *player = malloc(sizeof(player_t));

    if (player == NULL) {
        perror("Failed to allocate memory for player");
        return NULL;
    }
    srand(time(NULL));
    player->id = id;
    player->pos = pos;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    player->inventory = (inventory_t){0};
    player->team_id = team_id;
    return player;
}

void destroy_player(player_t *player)
{
    if (player == NULL)
        return;
    free(player);
}

void turn_player_left(player_t *player)
{
    player->orientation = turn_left(player->orientation);
    player->tick_cooldown = 7;
}

void turn_player_right(player_t *player)
{
    player->orientation = turn_right(player->orientation);
    player->tick_cooldown = 7;
}

/**
 * @brief Move the player forward in the direction they are facing.
 *
 * This function updates the player's position based on their current
 * orientation and the map's dimensions. If the player or map pointer is NULL,
 * it prints an error message and returns without making any changes.
 *
 * @param player Pointer to the player structure to be moved
 * @param map Pointer to the map structure where the player is located
 */
// TODO: Remove player pointer from the tile at the current position
//       Add player pointer to the tile at the new position
void move_player_forward(player_t *player, map_t *map)
{
    tile_t *current_tile = get_tile(map, player->pos);

    if (player == NULL || map == NULL || current_tile == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return;
    }
    remove_player_from_map(map, player);
    player->pos = get_forward_position(player->pos, player->orientation, map);
    player->tick_cooldown = 7;
    add_player_to_map(map, player);
}
