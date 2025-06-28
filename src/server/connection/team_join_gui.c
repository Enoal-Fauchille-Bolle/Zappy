/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI Team Join
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include "map/resources.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include "vector.h"
#include <string.h>

/**
 * @brief Displays information about all eggs in a team to a specific client
 *
 * Iterates through all eggs in the specified team and sends their details
 * (ID, parent ID, position) to the client. Also logs debug information about
 * each egg.
 *
 * @param server Pointer to the server structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 * @param team Pointer to the team whose eggs are being displayed
 */
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
        send_to_client(server->clients[client_index - 2],
            "enw #%zu #%d %d %d\n", egg->id,
            egg->parent_id != 0 ? (int)egg->parent_id : -1, egg->pos.x,
            egg->pos.y);
        debug_map(server->options->debug,
            "Egg #%zu spawned by player %zu at (%d, %d)\n", egg->id,
            egg->parent_id, egg->pos.x, egg->pos.y);
    }
}

/**
 * @brief Displays information about all eggs in all teams to a specific client
 *
 * Iterates through all teams in the server's game and sends egg information
 * for each team to the specified client.
 *
 * @param server Pointer to the server structure
 * @param client_index Index of the client in the server's file descriptor
 * array
 */
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

static void display_player_info(
    server_t *server, player_t *player, int client_index)
{
    if (player == NULL || player->client == NULL) {
        return;
    }
    send_to_client(server->clients[client_index - 2],
        "pnw #%zu %d %d %d %u %s\n", player->id, player->pos.x, player->pos.y,
        player->orientation + 1, player->level, player->team->name);
    send_to_client(server->clients[client_index - 2],
        "pin #%zu %d %d %zu %zu %zu %zu %zu %zu %zu\n", player->id,
        player->pos.x, player->pos.y, player->inventory[FOOD],
        player->inventory[LINEMATE], player->inventory[DERAUMERE],
        player->inventory[SIBUR], player->inventory[MENDIANE],
        player->inventory[PHIRAS], player->inventory[THYSTAME]);
    send_to_client(server->clients[client_index - 2], "plv #%zu %d\n",
        player->id, player->level);
}

static void display_players_info(server_t *server, int client_index)
{
    player_t *player = NULL;

    for (size_t i = 0; i < server->game->next_player_id; i++) {
        player = get_player_by_id(server->game, i);
        if (player == NULL)
            continue;
        display_player_info(server, player, client_index);
    }
}

/**
 * @brief Sends a welcome message to a GUI client
 *
 * This function sends the initial connection response to a GUI client,
 * including the current map state and team information.
 *
 * @param server Pointer to the server structure containing client file
 * descriptors and options
 * @param client_index Index of the GUI client in the server's file descriptor
 * array
 */
static void send_gui_welcome_message(server_t *server, int client_index)
{
    msz_command(server->clients[client_index - 2], NULL);
    sgt_command(server->clients[client_index - 2], NULL);
    mct_command(server->clients[client_index - 2], NULL);
    tna_command(server->clients[client_index - 2], NULL);
    display_players_info(server, client_index);
    display_eggs_info(server, client_index);
}

/**
 * @brief Handles a GUI client connection
 *
 * If the team name is "GRAPHIC", creates a new GUI client, sends a welcome
 * message, and returns SUCCESS. Otherwise, returns FAILURE.
 *
 * @param server Pointer to the server structure
 * @param team_name Name of the team the client wants to join
 * @param client_index Index of the client in the server's file descriptor
 * array
 *
 * @return true if the GUI client was successfully handled, false otherwise
 */
bool handle_gui_client(
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
