/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Check End
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/game_state.h"
#include "team/player/player.h"
#include "vector.h"
#include <stddef.h>

/**
 * @brief Check if a team has met the winning conditions
 *
 * This function checks if a team has enough players at the maximum level
 * to win the game. It also prints debug information if enabled.
 *
 * @param team Pointer to the team to check
 * @param debug Boolean indicating whether to print debug information
 * @return true if the team has won, false otherwise
 */
static bool check_team_end(team_t *team, bool debug)
{
    const vector_vtable_t *vtable = vector_get_vtable(team->players);
    size_t count = 0;
    player_t *player = NULL;

    if (team == NULL || team->players == NULL)
        return false;
    for (size_t i = 0; i < vtable->size(team->players); i++) {
        player = *(player_t **)vtable->at(team->players, i);
        if (player == NULL || player->level < MAX_PLAYER_LEVEL)
            return false;
        count++;
    }
    if (count >= TEAM_WIN_MAX_LEVEL_PLAYERS_REQUIRED) {
        debug_game(debug,
            "Team %s has won the game with %zu players at level %d.\n",
            team->name, count, MAX_PLAYER_LEVEL);
        return true;
    }
    return false;
}

/**
 * @brief Check if any team has met the winning conditions
 *
 * This function iterates through all teams in the game and checks if any
 * team has won. It returns the winning team if found, or NULL otherwise.
 *
 * @param game Pointer to the game structure containing teams
 * @return Pointer to the winning team, or NULL if no team has won
 */
static team_t *check_end(game_t *game)
{
    team_t *team = NULL;

    if (game == NULL || game->teams == NULL)
        return NULL;
    for (int i = 0; game->teams[i] != NULL; i++) {
        team = game->teams[i];
        if (check_team_end(team, game->server->options->debug)) {
            return team;
        }
    }
    return NULL;
}

/**
 * @brief Update the game state to end if a team has won
 *
 * This function checks if any team has met the winning conditions and updates
 * the game state to GAME_END if a winning team is found. It also sends an
 * event to the server indicating the winning team.
 *
 * @param game Pointer to the game structure containing teams and server
 */
void update_end(game_t *game)
{
    team_t *winning_team = check_end(game);

    if (winning_team == NULL)
        return;
    seg_event(winning_team, game->server);
    game->game_state = GAME_END;
}
