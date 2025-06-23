/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** sst Command
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "game/game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Checks if the tick speed is within the valid range.
 *
 * This function validates that the tick speed is between MIN_FREQUENCY and
 * MAX_FREQUENCY. If not, it sends an error message to the client.
 *
 * @param tick_speed The tick speed to check.
 * @param client The client that sent the command.
 * @return true if the tick speed is valid, false otherwise.
 */
static bool check_tick_speed(tick_rate_t tick_speed, client_t *client)
{
    if (tick_speed < MIN_FREQUENCY || tick_speed > MAX_FREQUENCY) {
        debug_warning(client->server->options->debug,
            "Invalid tick speed for sst command: %u\n", tick_speed);
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Checks if the command has the correct number of arguments.
 *
 * This function validates that the command has exactly one argument (the
 * player ID). If not, it sends an error message to the client.
 *
 * @param command The command structure containing the arguments.
 * @param client The client that sent the command.
 * @return true if the number of arguments is valid, false otherwise.
 */
static bool check_args_number(command_t *command, client_t *client)
{
    if (command->argc != 1) {
        debug_warning(client->server->options->debug,
            "Invalid number of arguments for sst command\n");
        write(client->sockfd, "sbp\n", 4);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Handles the sst command to set the server tick speed.
 *
 * This function checks the number of arguments, validates the tick speed,
 * updates the server's game tick rate, and sends a confirmation message to
 * the client.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the tick speed argument.
 */
void sst_command(client_t *client, command_t *command)
{
    tick_rate_t tick_speed = 0;

    if (!check_args_number(command, client))
        return;
    tick_speed = strtoul(command->argv[0], NULL, 10);
    if (!check_tick_speed(tick_speed, client))
        return;
    client->server->game->tick_rate = tick_speed;
    dprintf(client->sockfd, "sst %u\n", client->server->game->tick_rate);
    debug_game(client->server->options->debug,
        "sst command sent: %zu tick speed\n", client->server->game->tick_rate);
}
