/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Look Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "team/player/player.h"
#include "debug_categories.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void look_command(client_t *client, command_t *command)
{
    char *vision = look(client->player, client->server->game->map);

    (void)command;
    if (vision == NULL) {
        write(client->sockfd, "ko\n", 3);
        debug_player(client->server->options->debug,
            "Player %zu failed to look due to an error\n", client->player->id);
        return;
    }
    client->player->tick_cooldown = LOOK_COMMAND_COOLDOWN;
    dprintf(client->sockfd, "%s\n", vision);
    free(vision);
}
