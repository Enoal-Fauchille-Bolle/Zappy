/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <netinet/in.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdbool.h>

typedef struct server_s server_t;

void process_client_message(server_t *server, int client_index);

#endif /* !CLIENT_H_ */
