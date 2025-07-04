/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tna Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug_categories.h"
#include <stdbool.h>
#include <stdio.h>

void tna_command(client_t *client, command_t *command)
{
    (void)command;
    debug_game(client->server->options->debug,
        "Client %d: tna command sent: ", client->index);
    for (int i = 0; client->server->options->teams[i] != NULL; i++) {
        send_to_client(client, "tna %s\n", client->server->options->teams[i]);
        printf("'%s'", client->server->options->teams[i]);
        if (client->server->options->teams[i + 1] != NULL) {
            printf(", ");
        }
    }
    printf("\n");
}
