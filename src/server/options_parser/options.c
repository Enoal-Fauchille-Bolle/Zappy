/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Management
*/

#include "options_parser/options.h"
#include "options_parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Main entry point for parsing server command line options.
 *
 * This function orchestrates the complete option parsing process:
 * 1. Initializes the options structure
 * 2. Parses all command line arguments
 * 3. Validates that mandatory options are present
 * Sets error flag if any step fails.
 *
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 * @return Pointer to the populated server_options_t structure, or NULL on
 *         memory allocation failure
 *
 * @note Caller should check options->error flag before using the structure
 * @note Caller is responsible for freeing the returned structure
 */
server_options_t *get_server_options(int ac, char **av)
{
    server_options_t *options = init_options();

    if (options == NULL)
        return NULL;
    parse_options(options, ac, av);
    if (options->error)
        return options;
    if (!are_mandatory_options_present(options)) {
        fprintf(stderr, "Mandatory options are missing\n");
        options->error = true;
    }
    if (options->frequency == 0)
        options->frequency = 100;
    return options;
}

/**
 * @brief Destroys and frees memory allocated for server options structure
 *
 * This function safely deallocates all memory associated with a
 * server_options_t structure, including the teams array and individual team
 * name strings.
 *
 * @param options Pointer to the server_options_t structure to destroy.
 *                Can be NULL (function will return safely).
 */
void destroy_server_options(server_options_t *options)
{
    if (options == NULL)
        return;
    if (options->teams != NULL) {
        for (size_t i = 0; options->teams[i] != NULL; i++) {
            free(options->teams[i]);
        }
        free(options->teams);
    }
    free(options);
    options = NULL;
}
