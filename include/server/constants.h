/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Constants Header
*/

#ifndef CONSTANTS_H_
    #define CONSTANTS_H_

    #include <stdbool.h>

extern const bool SUCCESS;
extern const bool FAILURE;

extern const int EXIT_SUCCESS_CODE;
extern const int EXIT_ERROR_CODE;

    #define MAX_CLIENTS 128

    #define MAX_COMMAND_BUFFER_SIZE 10

    #define DEFAULT_TICK_RATE 100

    #define MIN_FREQUENCY 1
    #define MAX_FREQUENCY 1000

    #define MAX_MAP_SIZE 1000

    #define MAX_INITIAL_CLIENTS 1000000

#endif /* !CONSTANTS_H_ */
