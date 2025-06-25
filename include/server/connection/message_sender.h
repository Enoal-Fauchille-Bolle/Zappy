/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Message Sender Header
*/

#ifndef MESSAGE_SENDER_H_
    #define MESSAGE_SENDER_H_

    #include "connection/client.h"
#include "connection/server.h"

void queue_message_to_client(client_t *client, const char *format, ...);
void send_to_client(client_t *client, char *message);
void send_to_all_guis(server_t *server, const char *format, ...);

#endif /* !MESSAGE_SENDER_H_ */
