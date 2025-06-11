/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connection Handler Header
*/

#ifndef CONNECTION_HANDLER_H_
    #define CONNECTION_HANDLER_H_

    #include "connection/server.h"

void process_connections(server_t *server);
void remove_client(struct pollfd *fds, int client_index, bool debug);

#endif /* !CONNECTION_HANDLER_H_ */
