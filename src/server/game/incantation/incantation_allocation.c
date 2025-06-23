/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Allocation
*/

#include "command_handler/ai_commands.h"
#include "connection/client.h"
#include "game/game.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "map/tile.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

void destroy_incantation(incantation_t *incantation)
{
    if (incantation == NULL) {
        fprintf(stderr, "Invalid incantation pointer\n");
        return;
    }
    if (incantation->players != NULL) {
        free(incantation->players);
    }
    incantation = NULL;
}

void destroy_incantation_vector(vector_t *incantations)
{
    const vector_vtable_t *vtable = vector_get_vtable(incantations);
    size_t size = vtable->size(incantations);
    incantation_t *incantation = NULL;

    for (size_t i = 0; i < size; i++) {
        incantation = *(incantation_t **)vtable->at(incantations, i);
        destroy_incantation(incantation);
        free(incantation);
    }
    vector_destroy(incantations);
}

static void fill_players_array(player_t **players, tile_t *tile, level_t level,
    const vector_vtable_t *vtable)
{
    size_t count = 0;
    player_t *player = NULL;

    for (size_t i = 0; i < vtable->size(tile->players); i++) {
        player = *(player_t **)vtable->at(tile->players, i);
        if (player->level == level) {
            players[count] = player;
            count++;
        }
    }
    players[count] = NULL;
}

static player_t **init_players(game_t *game, pos_t pos, level_t level)
{
    const vector_vtable_t *vtable = vector_get_vtable(game->map->tiles);
    tile_t *tile = get_tile(game->map, pos);
    player_t **players =
        malloc(sizeof(player_t *) * (vtable->size(tile->players) + 1));

    if (players == NULL) {
        perror("Failed to allocate memory for players array");
        return NULL;
    }
    if (tile == NULL) {
        fprintf(stderr, "Tile not found at position (%d, %d)\n", pos.x, pos.y);
        free(players);
        return NULL;
    }
    fill_players_array(players, tile, level, vtable);
    return players;
}

incantation_t *create_incantation(game_t *game, pos_t pos, level_t level)
{
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    incantation_t *incantation = malloc(sizeof(incantation_t));

    if (incantation == NULL) {
        perror("Failed to allocate memory for incantation");
        return NULL;
    }
    incantation->players = init_players(game, pos, level);
    if (incantation->players == NULL) {
        fprintf(stderr, "Failed to initialize players for incantation\n");
        return incantation;
    }
    incantation->pos = pos;
    incantation->level = level;
    incantation->ticks_left = INCANTATION_COMMAND_COOLDOWN;
    vtable->push_back(game->incantations, &incantation);
    return incantation;
}
