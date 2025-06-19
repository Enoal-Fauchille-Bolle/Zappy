/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Right Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "map/orientation_names.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

void right_command(client_t *client, command_t *command)
{
    (void)command;
    turn_player_right(client->player);
    client->player->tick_cooldown = RIGHT_COMMAND_COOLDOWN;
    write(client->sockfd, "ok\n", 3);
    debug_player(client->server->options->debug, "Player %zu turned right\n",
        client->player->id);
    debug_map(client->server->options->debug, "Player %zu orientation: %s\n",
        client->player->id, orientation_names[client->player->orientation]);
}
