/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Constants Header
*/

#ifndef GAME_CONSTANTS_H_
    #define GAME_CONSTANTS_H_

    #include "map/resources.h"
    #include <stddef.h>

    #define GAME_TICK_DEBUG_INTERVAL 100
    #define GAME_RESOURCE_SPAWN_INTERVAL 20

    #define MAX_PLAYER_LEVEL 8

    #define FORWARD_COMMAND_COOLDOWN 7
    #define LEFT_COMMAND_COOLDOWN 7
    #define RIGHT_COMMAND_COOLDOWN 7
    #define LOOK_COMMAND_COOLDOWN 7
    #define INVENTORY_COMMAND_COOLDOWN 1
    #define BROADCAST_COMMAND_COOLDOWN 7
    #define CONNECT_NBR_COMMAND_COOLDOWN 0
    #define FORK_COMMAND_COOLDOWN 42
    #define EJECT_COMMAND_COOLDOWN 7
    #define TAKE_COMMAND_COOLDOWN 7
    #define SET_COMMAND_COOLDOWN 7
    #define INCANTATION_COMMAND_COOLDOWN 300

#endif /* !GAME_CONSTANTS_H_ */
