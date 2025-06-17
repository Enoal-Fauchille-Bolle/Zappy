/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game File
*/

#include "game/game.h"
#include "constants.h"
#include "debug_categories.h"
#include "game/game_state.h"
#include "game/teams.h"
#include "game/tick.h"
#include "map/map.h"
#include "map/resources.h"
#include "options_parser/options.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Destroys a game instance and frees all associated memory
 *
 * This function safely deallocates a game structure and all its components,
 * including the map, teams array, and individual team structures. It performs
 * null checks to prevent segmentation faults.
 *
 * @param game Pointer to the game structure to destroy (can be NULL)
 */
void destroy_game(game_t *game)
{
    if (game == NULL)
        return;
    if (game->teams != NULL) {
        for (int i = 0; game->teams[i] != NULL; i++) {
            destroy_team(game->teams[i]);
        }
        free(game->teams);
    }
    if (game->map != NULL)
        destroy_map(game->map);
    free(game);
    game = NULL;
}

/**
 * @brief Creates an array of teams from team names
 *
 * Allocates memory for an array of team pointers and creates a team
 * for each name provided. The resulting array is NULL-terminated.
 *
 * @param teams_name Array of team names (NULL-terminated)
 * @return team_t** Pointer to array of team pointers, or NULL on failure
 */
static team_t **create_teams(char **teams_name)
{
    int teams_count = get_teams_name_count(teams_name);
    team_t **team_array = malloc(sizeof(team_t) * (teams_count + 1));

    if (!team_array)
        return NULL;
    for (int i = 0; i < teams_count; i++) {
        team_array[i] = create_team(teams_name[i]);
    }
    team_array[teams_count] = NULL;
    return team_array;
}

/**
 * @brief Initializes a game instance with the provided server options
 *
 * This function sets up all the necessary components for a new game including
 * the map, teams, and initial game state parameters.
 *
 * @param game Pointer to the game structure to initialize
 * @param options Pointer to server options containing game configuration
 * @return true on successful initialization, false on failure
 *
 * @note On failure, allocated resources are automatically cleaned up
 */
static bool init_game(game_t *game, server_options_t *options)
{
    game->map = create_map(options->width, options->height);
    if (!game->map) {
        destroy_game(game);
        return FAILURE;
    }
    game->teams = create_teams(options->teams);
    if (!game->teams) {
        destroy_game(game);
        return FAILURE;
    }
    game->next_player_id = 1;
    game->tick_rate = options->frequency;
    game->game_tick = 0;
    game->game_state = GAME_RUNNING;
    return SUCCESS;
}

/**
 * @brief Sets up the game by spawning minimum required eggs for all teams
 *
 * This function initializes the game by iterating through all teams and
 * spawning the minimum number of eggs required based on the client count.
 *
 * @param game Pointer to the game structure containing teams and map
 * @param options Pointer to server options containing client number
 * configuration
 * @return bool Returns SUCCESS (true) if setup completed successfully
 */
static bool setup_game(game_t *game, server_options_t *options)
{
    for (int i = 0; game->teams[i] != NULL; i++) {
        spawn_min_eggs(game->map, game->teams[i], options->clients_nb);
    }
    spread_resources(game->map, options->debug);
    return SUCCESS;
}

/**
 * @brief Advances the game tick counter
 *
 * This function increments the game tick counter, which is used to track
 * the progression of the game state over time.
 *
 * @param game Pointer to the game structure containing the current tick count
 */
// TODO: Implement game tick logic
void game_tick(game_t *game, bool debug)
{
    if (game->game_tick % GAME_TICK_DEBUG_INTERVAL == 0)
        debug_game(debug, "Game tick %u", game->game_tick);
    update_players_ticks(game);
    spread_resources(game->map, debug);
    game->game_tick++;
}

/**
 * @brief Creates and initializes a new game instance
 *
 * Allocates memory for a game structure and initializes all its components
 * including the map, teams, and game state parameters based on the provided
 * server options.
 *
 * @param options Pointer to server options containing game configuration
 * @return Pointer to the created game instance, or NULL if creation failed
 *
 * @note The function performs proper cleanup on failure, freeing any
 *       partially allocated resources before returning NULL
 */
game_t *create_game(server_options_t *options)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game)
        return NULL;
    if (init_game(game, options) == FAILURE)
        return NULL;
    if (setup_game(game, options) == FAILURE) {
        destroy_game(game);
        return NULL;
    }
    return game;
}
