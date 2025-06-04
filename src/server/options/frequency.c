/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Frequency Option
*/

#include "options.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Handle the frequency option parsing from command line arguments.
 *
 * This function processes the "-f" option to set the server frequency
 * (time units per second). It validates that the frequency value is
 * within the valid range (10-10000) and sets an error flag if validation
 * fails.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets options->error to true if frequency value is missing or invalid
 * @note Frequency determines the speed of game simulation
 */
void handle_frequency(server_options_t *options, int *i, int ac, char **av)
{
    if (*i + 1 < ac) {
        *i += 1;
        options->frequency = atoi(av[*i]);
        if (options->frequency < 10 || options->frequency > 10000) {
            fputs("Error: Frequency must be a number between 10 and 10000\n",
                stderr);
            options->error = true;
        }
    } else {
        fputs("Error: Missing frequency value\n", stderr);
        options->error = true;
    }
}
