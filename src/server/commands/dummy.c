/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Dummy Command
*/

#include "command_handler/command.h"
#include "command_handler/command_status.h"
#include "debug.h"
#include <stdbool.h>
#include <unistd.h>

command_status_t dummy_command(
    command_t *command, int client_sockfd, bool debug)
{
    (void)command;
    write(client_sockfd, "ok\n", 3);
    debug_info(debug, "Executed dummy command\n");
    return COMMAND_SUCCESS;
}
