/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** enw Command
*/

#include "connection/client.h"
#include "connection/message_sender.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void enw_event(egg_t *egg)
{
    if (egg == NULL || egg->team == NULL)
        return;
    send_to_all_guis(egg->team->game->server, "enw #%zu #%d %d %d\n", egg->id,
        egg->parent_id != 0 ? (int)egg->parent_id : -1, egg->pos.x,
        egg->pos.y);
}
