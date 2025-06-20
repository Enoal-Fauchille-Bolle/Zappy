/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ebo Command
*/

#include "connection/client.h"
#include "connection/client_message.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void ebo_command(egg_t *egg)
{
    if (egg == NULL)
        return;
    send_to_all_guis(egg->team->game->server, "ebo %zu\n", egg->id);
}
