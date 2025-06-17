/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team Joining File
*/

#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/teams.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Validates if a client can join a specified team
 *
 * Checks if the team exists and has available slots (eggs) for new players.
 * Sends "ko" response to client and logs debug information on validation
 * failure.
 *
 * @param server Pointer to the server structure
 * @param team Pointer to the team to validate (can be NULL)
 * @param team_name Name of the team being joined (for logging)
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @return SUCCESS if team is valid and has available slots, FAILURE otherwise
 */
static bool validate_team(
    server_t *server, team_t *team, const char *team_name, int client_index)
{
    if (team == NULL) {
        debug_conn(server->options->debug,
            "Client %d tried to join invalid team '%s'",
            client_index - 2, team_name);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    if (get_egg_count(team) == 0) {
        debug_conn(server->options->debug,
            "Client %d tried to join full team '%s'",
            client_index - 2, team->name);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Assigns a client to a specific team and creates a new client instance
 *
 * Creates a new client for the given team and assigns it to the server's
 * client array at the specified index. Also logs debug information about the
 * assignment.
 *
 * @param server Pointer to the server structure
 * @param team Pointer to the team to assign the client to
 * @param client_index Index in the server's client array where the new client
 * will be stored
 * @return true on successful assignment (always returns SUCCESS)
 */
static bool assign_team(server_t *server, team_t *team, int client_index)
{
    server->clients[client_index - 2] =
        create_client(server, team, client_index);
    debug_conn(server->options->debug,
        "Player %d (Client %d) assigned to team '%s'",
        server->clients[client_index - 2]->player->id,
        client_index - 2,
        server->clients[client_index - 2]->team_name);
    return SUCCESS;
}

/**
 * @brief Parses and validates a team name from a client connection message
 *
 * Validates the team name against server rules and assigns the client to the
 * team if valid. Sends appropriate response messages to the client including
 * team capacity and world dimensions.
 *
 * @param server Pointer to the server structure
 * @param message The team name message received from the client
 * @param client_index Index of the client in the server's file descriptor
 * array
 *
 * @return true if team name is valid and client was successfully assigned
 * @return false if team name is invalid
 *
 * @note Currently sends "0" as team capacity placeholder - needs
 * implementation to send actual available slots for the team
 */
static bool validate_and_assign_team(
    server_t *server, const char *team_name, int client_index)
{
    team_t *team = get_team_from_name(server->game->teams, team_name);

    if (validate_team(server, team, team_name, client_index) == FAILURE) {
        return FAILURE;
    }
    if (assign_team(server, team, client_index) == FAILURE) {
        debug_warning(server->options->debug,
            "Failed to assign team '%s' to client %d", team_name,
            client_index - 2);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Sends a welcome message to a newly connected client
 *
 * This function sends the initial connection response to a client,
 * including a status code and the game world dimensions.
 *
 * @param server Pointer to the server structure containing client file
 * descriptors and options
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @return true on success (always returns SUCCESS)
 */
static bool send_welcome_message(server_t *server, int client_index)
{
    team_t *team = get_team_from_name(
        server->game->teams, server->clients[client_index - 2]->team_name);

    dprintf(server->fds[client_index].fd, "%ld\n", get_egg_count(team));
    dprintf(server->fds[client_index].fd, "%ld %ld\n", server->options->width,
        server->options->height);
    return SUCCESS;
}

/**
 * @brief Handles a client's request to join a team
 *
 * This function validates and assigns a client to the specified team,
 * then sends a welcome message upon successful assignment.
 *
 * @param server Pointer to the server structure
 * @param team_name Name of the team the client wants to join
 * @param client_index Index of the client in the server's file descriptor
 * array
 *
 * @return true on success (team assigned and welcome message sent)
 * @return false on failure (team assignment failed or welcome message failed)
 */
bool handle_team_join(
    server_t *server, const char *team_name, int client_index)
{
    if (!validate_and_assign_team(server, team_name, client_index)) {
        debug_warning(server->options->debug,
            "Failed to assign team '%s' to client %d", team_name,
            client_index - 2);
        return FAILURE;
    }
    if (send_welcome_message(server, client_index) == FAILURE) {
        debug_warning(server->options->debug,
            "Failed to send welcome message to client %d",
            client_index - 2);
        return FAILURE;
    }
    return SUCCESS;
}
