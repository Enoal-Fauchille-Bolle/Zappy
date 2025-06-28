/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AI Team Join
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/teams.h"
#include "map/resources.h"
#include "team/player/player.h"
#include "team/team.h"

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
        write_to_client(NULL, "ko\n", server->fds[client_index].fd);
        return FAILURE;
    }
    if (get_egg_count(team) == 0) {
        debug_conn(server->options->debug,
            "Client %d tried to join full team '%s'\n", client_index - 2,
            team->name);
        write_to_client(NULL, "ko\n", server->fds[client_index].fd);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Sends a new player information to all GUI clients
 *
 * This function sends the new player's information (ID, position, inventory)
 * to all GUI clients connected to the server. It formats the message according
 * to the Zappy protocol.
 *
 * @param server Pointer to the server structure
 * @param player Pointer to the player structure containing player information
 */
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
        send_to_client(server->clients[client_index - 2], "ko\n");
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
bool validate_and_assign_team(
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
bool send_ai_welcome_message(server_t *server, int client_index)
{
    team_t *team = server->clients[client_index - 2]->player->team;

    send_to_client(
        server->clients[client_index - 2], "%ld\n", get_egg_count(team));
    send_to_client(server->clients[client_index - 2], "%ld %ld\n",
        server->options->width, server->options->height);
    return SUCCESS;
}
