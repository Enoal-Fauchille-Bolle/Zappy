/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team Joining File
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/message_receiver.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game_state.h"
#include "game/teams.h"
#include "map/resources.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>
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
            "Client %d tried to join invalid team '%s'\n", client_index - 2,
            team_name);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    if (get_egg_count(team) == 0) {
        debug_conn(server->options->debug,
            "Client %d tried to join full team '%s'\n", client_index - 2,
            team->name);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    return SUCCESS;
}

static void send_new_player_to_guis(server_t *server, player_t *player)
{
    send_to_all_guis(server, "pin #%zu %d %d %zu %zu %zu %zu %zu %zu\n",
        player->id, player->pos.x, player->pos.y, player->inventory[FOOD],
        player->inventory[LINEMATE], player->inventory[DERAUMERE],
        player->inventory[SIBUR], player->inventory[MENDIANE],
        player->inventory[PHIRAS], player->inventory[THYSTAME]);
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
    if (server->clients[client_index - 2] == NULL) {
        debug_warning(server->options->debug,
            "Failed to create client for team '%s' at index %d\n", team->name,
            client_index - 2);
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    pnw_event(server->clients[client_index - 2]->player);
    send_new_player_to_guis(server, server->clients[client_index - 2]->player);
    debug_game(server->options->debug,
        "Player %d (Client %d) assigned to team '%s'\n",
        server->clients[client_index - 2]->player->id, client_index - 2,
        team->name);
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
            "Failed to assign team '%s' to client %d\n", team_name,
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
static bool send_ai_welcome_message(server_t *server, int client_index)
{
    team_t *team = server->clients[client_index - 2]->player->team;

    dprintf(server->fds[client_index].fd, "%ld\n", get_egg_count(team));
    dprintf(server->fds[client_index].fd, "%ld %ld\n", server->options->width,
        server->options->height);
    return SUCCESS;
}

static void display_team_eggs_info(
    server_t *server, int client_index, team_t *team)
{
    const vector_vtable_t *vtable = vector_get_vtable(team->eggs);
    egg_t *egg = NULL;

    for (size_t i = 0; i < get_egg_count(team); i++) {
        egg = *(egg_t **)vtable->at(team->eggs, i);
        if (egg == NULL) {
            continue;
        }
        dprintf(server->fds[client_index].fd, "enw #%zu #%d %d %d\n", egg->id,
            egg->parent_id != 0 ? (int)egg->parent_id : -1, egg->pos.x,
            egg->pos.y);
        debug_map(server->options->debug,
            "Egg #%zu spawned by player #%zu at (%d, %d)\n", egg->id,
            egg->parent_id, egg->pos.x, egg->pos.y);
    }
}

static void display_eggs_info(server_t *server, int client_index)
{
    team_t *team = NULL;

    for (size_t i = 0; server->game->teams[i] != NULL; i++) {
        team = server->game->teams[i];
        if (team == NULL) {
            continue;
        }
        display_team_eggs_info(server, client_index, team);
    }
}

static void send_gui_welcome_message(server_t *server, int client_index)
{
    msz_command(server->clients[client_index - 2], NULL);
    sgt_command(server->clients[client_index - 2], NULL);
    mct_command(server->clients[client_index - 2], NULL);
    tna_command(server->clients[client_index - 2], NULL);
    display_eggs_info(server, client_index);
}

static bool handle_gui_client(
    server_t *server, const char *team_name, int client_index)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        server->clients[client_index - 2] =
            create_client(server, NULL, client_index);
        if (server->clients[client_index - 2] == NULL) {
            debug_warning(server->options->debug,
                "Failed to create GUI client at index %d\n", client_index - 2);
            return FAILURE;
        }
        send_gui_welcome_message(server, client_index);
        return SUCCESS;
    }
    return FAILURE;
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
    if (server->game->game_state == GAME_END) {
        write(server->fds[client_index].fd, "ko\n", 3);
        return FAILURE;
    }
    if (handle_gui_client(server, team_name, client_index) == SUCCESS) {
        return SUCCESS;
    }
    if (!validate_and_assign_team(server, team_name, client_index)) {
        debug_warning(server->options->debug,
            "Failed to assign team '%s' to client %d\n", team_name,
            client_index - 2);
        return FAILURE;
    }
    if (send_ai_welcome_message(server, client_index) == FAILURE) {
        debug_warning(server->options->debug,
            "Failed to send welcome message to client %d\n", client_index - 2);
        return FAILURE;
    }
    return SUCCESS;
}
