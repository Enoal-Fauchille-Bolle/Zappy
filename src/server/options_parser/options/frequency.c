/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Frequency Option
*/

#include "constants.h"
#include "options_parser/options.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Handle the frequency option parsing from command line arguments.
 *
 * This function processes the "-f" option to set the server frequency
 * (time units per second). It validates that the frequency value is
 * within the valid range (MIN_FREQUENCY-MAX_FREQUENCY) and sets an error flag
 * if validation fails.
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
    if (options->frequency != 0) {
        fputs("Error: Frequency option already set\n", stderr);
        options->error = true;
        *i += 1;
        return;
    }
    if (*i + 1 < ac) {
        *i += 1;
        options->frequency = atoi(av[*i]);
        if (options->frequency < MIN_FREQUENCY ||
            options->frequency > MAX_FREQUENCY) {
            fprintf(stderr,
                "Error: Frequency must be a number between %d and %d\n",
                MIN_FREQUENCY, MAX_FREQUENCY);
            options->error = true;
        }
    } else {
        fputs("Error: Missing frequency value\n", stderr);
        options->error = true;
    }
}
