/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connection Handler
*/

#include "connection/connection_handler.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"

static void process_client_events(
    struct pollfd *fds, int max_fds, client_t **clients)
{
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0)
            continue;
        if (fds[i].revents & POLLIN) {
            process_client_message(&fds[i], clients[i]);
        }
    }
}

static void accept_new_connection(server_t *server, struct pollfd *fds)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(
        server->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_sockfd == -1)
        return perror("accept");
    if (server->options->debug)
        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        if (fds[i].fd < 0) {
            server->clients[i] =
                init_client(server, client_sockfd, &client_addr);
            fds[i].fd = client_sockfd;
            fds[i].events = POLLIN;
            break;
        }
    }
}

// TODO: if (fds[0].revents & POLLIN && server->game->game_state == GAME_START)
static bool process_connection(server_t *server, struct pollfd *fds)
{
    int result = poll(fds, MAX_CLIENTS + 1, POLL_TIMEOUT);

    if (result < 0) {
        if (errno != EINTR)
            perror("poll");
        return FAILURE;
    }
    if (fds[0].revents & POLLIN) {
        accept_new_connection(server, fds);
    }
    process_client_events(fds, MAX_CLIENTS + 1, server->clients);
    return SUCCESS;
}

static void init_poll_fds(struct pollfd *fds, int server_sockfd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = server_sockfd;
    fds[0].events = POLLIN;
}

// TODO: Replace the "while (true)" with the actual game state check
// TODO:
// if (game is running)
//     game_tick(server);
void process_connections(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 1];

    init_poll_fds(fds, server->sockfd);
    while (true) {
        if (process_connection(server, fds) == FAILURE)
            break;
    }
}
