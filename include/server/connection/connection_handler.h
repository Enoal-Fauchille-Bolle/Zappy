/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connection Handler Header
*/

#ifndef CONNECTION_HANDLER_H_
    #define CONNECTION_HANDLER_H_

    #include "connection/server.h"
    #include <stdbool.h>

bool process_connection(server_t *server, int timeout_ms, bool *timed_out);

#endif /* !CONNECTION_HANDLER_H_ */
