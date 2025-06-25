/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Message Receiver Header
*/

#ifndef MESSAGE_RECEIVER_H_
    #define MESSAGE_RECEIVER_H_

    #include "connection/server.h"

void handle_client_message(server_t *server, int client_index);
void send_to_all_guis(server_t *server, const char *format, ...);

#endif /* !MESSAGE_RECEIVER_H_ */
