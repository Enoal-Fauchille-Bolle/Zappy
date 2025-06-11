/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Option Parser
*/

#include "options_parser/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const option_map_t options_map[] = {{"-p", handle_port},
    {"-x", handle_width}, {"-y", handle_height}, {"-n", handle_teams},
    {"-c", handle_clients}, {"-f", handle_frequency}, {"-h", handle_help},
    {"--help", handle_help}, {"-d", handle_debug}, {"--debug", handle_debug},
    {NULL, NULL}};
static const server_options_t default_options = {
    .port = 0,
    .width = 0,
    .height = 0,
    .teams = NULL,
    .clients_nb = 0,
    .frequency = 0,
    .help = false,
    .debug = false,
    .error = false
};

/**
 * @brief Initialize server options structure with default values.
 *
 * This function allocates memory for a server_options_t structure and
 * initializes all fields to their default values. The structure is
 * used to store command line options for the server.
 *
 * @return Pointer to the initialized server_options_t structure on success,
 *         NULL if memory allocation fails
 *
 * @note Caller is responsible for freeing the returned structure
 * @note Default frequency is set to 100
 */
server_options_t *init_options(void)
{
    server_options_t *options = malloc(sizeof(server_options_t));

    if (options == NULL) {
        perror("Failed to allocate memory for server options");
        return NULL;
    }
    memcpy(options, &default_options, sizeof(default_options));
    return options;
}

/**
 * @brief Find the option map entry for a given command line argument.
 *
 * This function searches through the static option map array to find
 * a matching entry for the provided command line argument. The option
 * map contains pairs of option strings and their corresponding handler
 * functions.
 *
 * @param arg The command line argument string to search for
 * @return Pointer to the matching option_map_t entry if found, NULL otherwise
 *
 * @note Uses static array with string comparison for lookup
 */
static const option_map_t *find_option(const char *arg)
{
    for (const option_map_t *entry = options_map; entry->option != NULL;
        entry++) {
        if (strcmp(arg, entry->option) == 0)
            return entry;
    }
    return NULL;
}

/**
 * @brief Parse command line options and populate the options structure.
 *
 * This function iterates through all command line arguments starting from
 * index 1 (skipping program name) and processes each option using the
 * appropriate handler function. If an unknown option is encountered,
 * an error is set and parsing stops.
 *
 * @param options Pointer to the server options structure to populate
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets options->error to true if unknown option is encountered
 * @note Stops parsing on first error
 */
static void parse_options(server_options_t *options, int ac, char **av)
{
    const option_map_t *option_map = NULL;

    for (int i = 1; i < ac; i++) {
        option_map = find_option(av[i]);
        if (option_map != NULL && option_map->handler != NULL) {
            option_map->handler(options, &i, ac, av);
        } else {
            dprintf(fileno(stderr), "Unknown option: %s\n", av[i]);
            options->error = true;
            return;
        }
    }
}

/**
 * @brief Check if all mandatory options are present and valid.
 *
 * This function validates that all required command line options have
 * been provided and contain valid values. The mandatory options are:
 * port (> 0), width (> 0), height (> 0), and at least one team name.
 *
 * @param options Pointer to the server options structure to validate
 * @return true if all mandatory options are present and valid, false otherwise
 *
 * @note Does not validate option value ranges, only presence
 */
bool mandatory_options_present(server_options_t *options)
{
    if (options->help)
        return true;
    if (options->port <= 0 || options->width == 0 || options->height == 0 ||
        options->teams == NULL || options->teams[0] == NULL) {
        return false;
    }
    return true;
}

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
    if (!mandatory_options_present(options)) {
        fprintf(stderr, "Mandatory options are missing\n");
        options->error = true;
    }
    if (options->frequency == 0)
        options->frequency = 100;
    return options;
}

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
}
