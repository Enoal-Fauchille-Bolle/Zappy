/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Writing Buffer Header
*/

#ifndef WRITING_BUFFER_H_
    #define WRITING_BUFFER_H_

    #include "connection/client.h"
    #include <stdbool.h>

void add_message_to_buffer(client_t *client, char *message);
char *pop_message_from_buffer(client_t *client);
void handle_clients_writing_buffer(server_t *server);
void clear_writing_buffer(client_t *client);

#endif /* !WRITING_BUFFER_H_ */
