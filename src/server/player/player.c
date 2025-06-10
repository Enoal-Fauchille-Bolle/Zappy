/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "player/player.h"
#include "map/coordinates.h"
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
}

void turn_player_right(player_t *player)
{
    player->orientation = turn_right(player->orientation);
}
