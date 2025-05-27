/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Teams Option
*/

#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Handle the teams option parsing from command line arguments.
 *
 * This function processes the "-n" option to collect team names.
 * It reads consecutive arguments until it encounters another option
 * (starting with '-') or reaches the end of arguments. The team names
 * are stored in a dynamically allocated array of strings.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Allocates memory for teams array and duplicates team name strings
 * @note Sets options->error to true if no team names are provided
 * @note The teams array is NULL-terminated
 */
void handle_teams(server_options_t *options, int *i, int ac, char **av)
{
    int start = *i;
    int teams_count = 0;

    while (*i + 1 < ac && av[*i + 1][0] != '-') {
        *i += 1;
        teams_count++;
    }
    if (*i <= start) {
        dprintf(fileno(stderr), "Error: No teams provided.\n");
        options->error = true;
        return;
    }
    options->teams = malloc(sizeof(char *) * (teams_count + 1));
    for (int j = start + 1; j <= *i; j++) {
        options->teams[j - start - 1] = strdup(av[j]);
    }
    options->teams[teams_count] = NULL;
}
