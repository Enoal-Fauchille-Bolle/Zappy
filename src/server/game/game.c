/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game File
*/

#include "game/game.h"
#include "connection/server.h"
#include "constants.h"
#include "game/game_state.h"
#include "game/incantation.h"
#include "game/teams.h"
#include "map/map.h"
#include "map/resources.h"
#include "options_parser/options.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
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
    if (game->incantations != NULL) {
        destroy_incantation_vector(game->incantations);
    }
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
static team_t **create_teams(char **teams_name, game_t *game)
{
    int teams_count = get_teams_name_count(teams_name);
    team_t **team_array = malloc(sizeof(team_t) * (teams_count + 1));

    if (!team_array) {
        perror("Failed to allocate memory for teams array");
        return NULL;
    }
    for (int i = 0; i < teams_count; i++) {
        team_array[i] = create_team(teams_name[i], game);
    }
    team_array[teams_count] = NULL;
    return team_array;
}

/**
 * @brief Get a player by their ID within a specific team.
 *
 * This function searches for a player with the specified ID in the given team.
 * If the team or player vector is NULL, it prints an error message and returns
 * NULL. If a player with the specified ID is found, it returns a pointer to
 * that player; otherwise, it returns NULL.
 *
 * @param team Pointer to the team structure to search in
 * @param id The unique identifier of the player to find
 * @return Pointer to the player_t structure if found, NULL otherwise
 */
static player_t *get_player_by_id_in_team(team_t *team, size_t id)
{
    const vector_vtable_t *vtable;
    player_t *player;

    if (team == NULL) {
        fprintf(stderr, "Team pointer is NULL\n");
        return NULL;
    }
    vtable = vector_get_vtable(team->players);
    for (size_t i = 0; i < vtable->size(team->players); i++) {
        player = *(player_t **)vtable->at(team->players, i);
        if (player != NULL && player->id == id) {
            return player;
        }
    }
    return NULL;
}

/**
 * @brief Get a player by their ID from the game.
 *
 * This function searches through all teams in the game to find a player with
 * the specified ID. If the game pointer is NULL, it prints an error message
 * and returns NULL. If a player with the specified ID is found, it returns a
 * pointer to that player; otherwise, it returns NULL.
 *
 * @param game Pointer to the game structure containing teams and players
 * @param id The unique identifier of the player to find
 * @return Pointer to the player_t structure if found, NULL otherwise
 */
player_t *get_player_by_id(game_t *game, size_t id)
{
    player_t *player;

    if (game == NULL) {
        fprintf(stderr, "Game pointer is NULL\n");
        return NULL;
    }
    for (size_t i = 0; game->teams[i] != NULL; i++) {
        player = get_player_by_id_in_team(game->teams[i], id);
        if (player != NULL) {
            return player;
        }
    }
    return NULL;
}

/**
 * @brief Initializes game core components (map, teams, incantations)
 *
 * This function sets up the fundamental components of the game including
 * the map, teams array, and incantations vector.
 *
 * @param game Pointer to the game structure to initialize
 * @param server Pointer to server containing configuration options
 * @return true on successful initialization, false on failure
 *
 * @note On failure, allocated resources are automatically cleaned up
 */
static bool init_game_components(game_t *game, server_t *server)
{
    game->map =
        create_map(server->options->width, server->options->height, server);
    if (!game->map) {
        destroy_game(game);
        return FAILURE;
    }
    game->teams = create_teams(server->options->teams, game);
    if (!game->teams) {
        destroy_game(game);
        return FAILURE;
    }
    game->incantations = vector_new(sizeof(incantation_t *));
    if (!game->incantations) {
        destroy_game(game);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Initializes a game instance with the provided server options
 *
 * This function sets up all the necessary components for a new game including
 * the map, teams, and initial game state parameters.
 *
 * @param game Pointer to the game structure to initialize
 * @param server Pointer to server containing configuration options
 * @return true on successful initialization, false on failure
 *
 * @note On failure, allocated resources are automatically cleaned up
 */
static bool init_game(game_t *game, server_t *server)
{
    if (init_game_components(game, server) == FAILURE) {
        return FAILURE;
    }
    game->server = server;
    game->next_player_id = 1;
    game->next_egg_id = 1;
    game->tick_rate = server->options->frequency;
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
static bool setup_game(game_t *game, server_t *server)
{
    for (int i = 0; game->teams[i] != NULL; i++) {
        spawn_min_eggs(game->map, game->teams[i], server->options->clients_nb,
            server->options->debug);
    }
    spread_resources(game->map, server->options->debug);
    return SUCCESS;
}

/**
 * @brief Creates a new game instance and initializes it with the server
 * configuration
 *
 * This function allocates memory for a game structure, initializes it with
 * the server options, and sets up the game state. If any step fails, it cleans
 * up allocated resources and returns NULL.
 *
 * @param server Pointer to the server structure containing configuration
 * @return game_t* Pointer to the created game instance, or NULL on failure
 */
game_t *create_game(server_t *server)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game)
        return NULL;
    if (init_game(game, server) == FAILURE)
        return NULL;
    if (setup_game(game, server) == FAILURE) {
        destroy_game(game);
        return NULL;
    }
    return game;
}
