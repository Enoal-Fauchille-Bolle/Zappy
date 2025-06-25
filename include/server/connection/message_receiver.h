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

#endif /* !MESSAGE_RECEIVER_H_ */
