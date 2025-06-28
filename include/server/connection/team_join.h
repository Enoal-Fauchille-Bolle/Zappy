/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team Join Header
*/

#ifndef TEAM_JOIN_H_
    #define TEAM_JOIN_H_

    #include "connection/client.h"
    #include <stdbool.h>

bool handle_team_join(
    server_t *server, const char *team_name, int client_index);

// AI
bool validate_and_assign_team(
    server_t *server, const char *team_name, int client_index);
bool send_ai_welcome_message(server_t *server, int client_index);

// GUI
bool handle_gui_client(
    server_t *server, const char *team_name, int client_index);

#endif /* !TEAM_JOIN_H_ */
