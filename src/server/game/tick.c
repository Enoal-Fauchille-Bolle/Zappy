/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tick Management
*/

#include "connection/connection_handler.h"
#include "debug_categories.h"
#include "game/game.h"
#include "map/resources.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Handle player starvation by checking their food inventory
 *
 * This function checks if the player has food available. If not, it marks the
 * player as dead and removes them from the game. If the player has food, it
 * decrements their food inventory and resets the hunger cooldown.
 *
 * @param player Pointer to the player structure to check for starvation
 * @return true if the player has starved and is removed, false otherwise
 */
static bool handle_player_starvation(player_t *player)
{
    if (player->hunger_cooldown > 0) {
        player->hunger_cooldown--;
        return false;
    }
    if (player->inventory[FOOD] == 0) {
        debug_game(player->client->server->options->debug,
            "Player %zu (Client %d) has starved\n", player->id,
            player->client->index);
        write(player->client->sockfd, "dead\n", 5);
        remove_client(player->client->server, player->client->index + 2);
        return true;
    }
    player->inventory[FOOD]--;
    debug_player(player->client->server->options->debug,
        "Player %zu has eaten, remaining food: %zu\n", player->id,
        player->inventory[FOOD]);
    player->hunger_cooldown = 126;
    return false;
}

/**
 * @brief Update the tick cooldown for a player
 *
 * This function decrements the player's tick cooldown if it is greater than 0.
 * If the cooldown reaches 0, the player can act again.
 *
 * @param player Pointer to the player structure to update
 */
static void update_player_tick_cooldown(player_t *player)
{
    if (player->tick_cooldown > 0)
        player->tick_cooldown--;
}

/**
 * @brief Update the tick cooldown for all players in a team
 *
 * This function iterates through all players in a team and updates their
 * tick cooldown. It also checks for player starvation and handles it if
 * necessary.
 *
 * @param team Pointer to the team structure containing players
 */
static void update_players_team_ticks(team_t *team)
{
    const vector_vtable_t *vtable = vector_get_vtable(team->players);
    player_t *player = NULL;
    size_t i = 0;

    while (i < vtable->size(team->players)) {
        player = *(player_t **)vtable->at(team->players, i);
        if (player == NULL) {
            i++;
            continue;
        }
        update_player_tick_cooldown(player);
        if (handle_player_starvation(player)) {
            continue;
        }
        i++;
    }
}

/**
 * @brief Update the tick cooldown for all players in the game
 *
 * This function iterates through all teams in the game and updates the tick
 * cooldown for each player. It also checks for player starvation and handles
 * it accordingly.
 *
 * @param game Pointer to the game structure containing teams and players
 */
void update_players_ticks(game_t *game)
{
    for (int i = 0; game->teams[i] != NULL; i++) {
        update_players_team_ticks(game->teams[i]);
    }
}
