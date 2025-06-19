/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Dummy Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug.h"
#include <stdbool.h>
#include <unistd.h>

void dummy_command(client_t *client, command_t *command)
{
    (void)command;
    write(client->sockfd, "ok\n", 3);
    debug_info(client->server->options->debug, "Executed dummy command\n");
}
