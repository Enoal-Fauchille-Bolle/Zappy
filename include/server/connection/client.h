/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

typedef struct server_s server_t;

void handle_client_message(server_t *server, int client_index);

#endif /* !CLIENT_H_ */
