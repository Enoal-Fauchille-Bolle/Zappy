/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team Joining File
*/

#include "connection/team_join.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "game/game_state.h"
#include "team/team.h"

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
        send_to_client(server->clients[client_index - 2], "ko\n");
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
