/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Message Handler Header
*/

#ifndef CLIENT_MESSAGE_H_
    #define CLIENT_MESSAGE_H_

    #include "connection/server.h"

void handle_client_message(server_t *server, int client_index);
void send_to_all_guis(server_t *server, const char *format, ...);

#endif /* !CLIENT_MESSAGE_H_ */
