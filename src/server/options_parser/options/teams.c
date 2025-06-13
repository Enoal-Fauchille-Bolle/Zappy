/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Teams Option
*/

#include "constants.h"
#include "options_parser/options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialize the teams array in server options structure.
 *
 * This function allocates memory for an array of strings to hold team names.
 * The size of the array is determined by the number of teams specified.
 * It initializes each element to NULL and ensures the last element is also
 * NULL to mark the end of the array.
 *
 * @param teams_count Number of teams to allocate space for
 * @return Pointer to the allocated array of strings, or NULL on failure
 */
static char **init_teams(int teams_count)
{
    char **teams = malloc(sizeof(char *) * (teams_count + 1));

    if (teams == NULL) {
        perror("Failed to allocate memory for teams");
        return NULL;
    }
    for (int i = 0; i < teams_count; i++) {
        teams[i] = NULL;
    }
    teams[teams_count] = NULL;
    return teams;
}

/**
 * @brief Check if a team name already exists in the options structure.
 *
 * This function iterates through the teams array in the server options
 * structure and checks if the specified team name already exists.
 * It returns true if the team name is found, false otherwise.
 *
 * @param options Pointer to the server options structure containing teams
 * @param team_name The name of the team to check for existence
 * @return true if the team name already exists
 * @return false if the team name does not exist
 */
static bool do_team_already_exists(
    server_options_t *options, const char *team_name)
{
    for (int i = 0; options->teams && options->teams[i]; i++) {
        if (strcmp(options->teams[i], team_name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Advances the argument index to the end of a multi-value option and
 * counts teams
 *
 * This function iterates through command line arguments starting from the
 * current position until it encounters the next option (argument starting with
 * '-') or reaches the end of the argument list. For each non-option argument
 * encountered, it increments both the argument index and the teams counter.
 *
 * @param i Pointer to the current argument index (modified in-place)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 * @param teams_count Pointer to the teams counter (incremented for each team
 * found)
 *
 * @note The function modifies both *i and *teams_count by reference
 * @note Stops when reaching end of arguments or encountering an option
 * starting with '-'
 */
static void go_to_end_of_option(int *i, int ac, char **av, int *teams_count)
{
    while (*i + 1 < ac && av[*i + 1][0] != '-') {
        *i += 1;
        (*teams_count)++;
    }
}

/**
 * @brief Destroys and deallocates the teams array in server options
 *
 * This function safely deallocates all memory associated with the teams array
 * in the server options structure. It first checks if the teams array exists,
 * then iterates through each team name string to free individual allocations,
 * finally freeing the array itself and setting the pointer to NULL.
 *
 * @param options Pointer to the server options structure containing the teams
 * array
 *
 * @note This function handles NULL pointer safety by checking if
 * options->teams is NULL
 * @note After execution, options->teams will be set to NULL to prevent
 * dangling pointers
 */
static void destroy_teams(server_options_t *options)
{
    if (options->teams == NULL)
        return;
    for (int i = 0; options->teams[i] != NULL; i++) {
        free(options->teams[i]);
    }
    free(options->teams);
    options->teams = NULL;
}

/**
 * @brief Fill the teams array with team names from command line arguments.
 *
 * This function iterates over the command line arguments starting from
 * the specified start index to the end index. It checks if each team
 * name already exists in the options structure. If a team name
 * already exists, it prints an error message and cleans up the teams
 * array before returning false. If the team name is new, it duplicates
 * the string and stores it in the teams array. If memory allocation
 * fails, it prints an error message, cleans up the teams array, and
 * returns false.
 *
 * @param options Pointer to the server options structure to modify
 * @param av Array of command line argument strings
 * @param start Starting index in the command line arguments
 * @param end Ending index in the command line arguments
 * @return true if all team names are successfully added,
 * @return false if a team name already exists or memory allocation fails.
 */
static bool fill_teams(
    server_options_t *options, char **av, int start, int end)
{
    for (int j = start + 1; j <= end; j++) {
        if (do_team_already_exists(options, av[j])) {
            dprintf(
                fileno(stderr), "Error: Team '%s' already exists.\n", av[j]);
            destroy_teams(options);
            return FAILURE;
        }
        options->teams[j - start - 1] = strdup(av[j]);
        if (options->teams[j - start - 1] == NULL) {
            perror("Failed to allocate memory for team name");
            destroy_teams(options);
            return FAILURE;
        }
    }
    return SUCCESS;
}

/**
 * @brief Handles error when teams option is already set
 *
 * Prints an error message to stderr, sets the error flag in options,
 * and skips remaining team arguments until the next option or end of
 * arguments.
 *
 * @param options Pointer to server options structure
 * @param i Pointer to current argument index
 * @param ac Argument count
 * @param av Argument vector
 */
static void already_set_teams_error(
    server_options_t *options, int *i, int ac, char **av)
{
    fputs("Error: Teams option already set\n", stderr);
    options->error = true;
    while (*i + 1 < ac && av[*i + 1][0] != '-')
        *i += 1;
}

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

    if (options->teams != NULL) {
        already_set_teams_error(options, i, ac, av);
        return;
    }
    go_to_end_of_option(i, ac, av, &teams_count);
    if (*i <= start) {
        fputs("Error: No teams provided.\n", stderr);
        options->error = true;
        return;
    }
    options->teams = init_teams(teams_count);
    if (!fill_teams(options, av, start, *i)) {
        options->error = true;
        return;
    }
    options->teams[teams_count] = NULL;
}
