/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser Test Utilities
*/

#ifndef OPTIONS_PARSER_TEST_UTILS_H_
#define OPTIONS_PARSER_TEST_UTILS_H_

#include "options_parser/options.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function to cleanup server options structure
 */
static inline void cleanup_server_options(server_options_t *options)
{
    if (options) {
        destroy_server_options(options);
    }
}

/**
 * @brief Helper function to create teams array for testing
 */
static inline char **create_teams_array(const char **team_names, size_t count)
{
    char **teams = malloc(sizeof(char *) * (count + 1));

    if (!teams)
        return NULL;

    for (size_t i = 0; i < count; i++) {
        teams[i] = strdup(team_names[i]);
        if (!teams[i]) {
            for (size_t j = 0; j < i; j++) {
                free(teams[j]);
            }
            free(teams);
            return NULL;
        }
    }
    teams[count] = NULL;
    return teams;
}

#endif /* !OPTIONS_PARSER_TEST_UTILS_H_ */
