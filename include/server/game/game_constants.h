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
    #define GAME_TICK_FOOD_COOLDOWN 126

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

static const inventory_t INCANT_REQS[MAX_PLAYER_LEVEL - 1] = {
    [0] = {0, 1, 0, 0, 0, 0, 0},      // Level 1 to 2
    [1] = {0, 1, 1, 1, 0, 0, 0},      // Level 2 to 3
    [2] = {0, 2, 0, 1, 0, 2, 0},      // Level 3 to 4
    [3] = {0, 1, 1, 2, 0, 1, 0},      // Level 4 to 5
    [4] = {0, 1, 2, 1, 3, 0, 0},      // Level 5 to 6
    [5] = {0, 1, 2, 3, 0, 1, 0},      // Level 6 to 7
    [6] = {0, 2, 2, 2, 2, 2, 1}       // Level 7 to 8
};

static const size_t NB_PLAYERS_REQUIRED[MAX_PLAYER_LEVEL - 1] = {
    [0] = 1,      // Level 1 to 2
    [1] = 2,      // Level 2 to 3
    [2] = 2,      // Level 3 to 4
    [3] = 4,      // Level 4 to 5
    [4] = 4,      // Level 5 to 6
    [5] = 6,      // Level 6 to 7
    [6] = 6       // Level 7 to 8
};

#endif /* !GAME_CONSTANTS_H_ */
