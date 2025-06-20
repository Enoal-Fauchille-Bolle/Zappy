/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Factory
*/

#include "command_handler/command_factory.h"
#include "command_handler/command.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Destroy a command and free allocated resources.
 *
 * This function frees the memory associated with the command structure
 * and its arguments.
 *
 * @param command The command structure to destroy.
 */
void destroy_command(command_t *command)
{
    if (command == NULL)
        return;
    if (command->tokens != NULL) {
        for (int i = 0; command->tokens[i] != NULL; i++) {
            free(command->tokens[i]);
        }
        free(command->tokens);
    }
    free(command);
    command = NULL;
}

/**
 * @brief Initialize a command structure.
 *
 * This function allocates memory for a command structure and initializes
 * its members to default values.
 *
 * @return command_t* Pointer to the initialized command structure.
 */
static command_t *init_command(void)
{
    command_t *command = malloc(sizeof(command_t));

    if (command == NULL) {
        perror("malloc");
        return NULL;
    }
    command->name = NULL;
    command->argc = -1;
    command->argv = NULL;
    command->tokens = NULL;
    return command;
}

/**
 * @brief Set up the command structure with the parsed tokens.
 *
 * This function initializes the command structure with the command name,
 * argument count, and argument values.
 *
 * @param command The command structure to set up.
 * @param tokens The array of tokens parsed from the buffer.
 * @param parsed_count The actual number of tokens parsed.
 */
static void setup_command(command_t *command, char **tokens, int parsed_count)
{
    if (tokens == NULL || parsed_count <= 0) {
        command->name = NULL;
        command->argc = -1;
        command->argv = NULL;
        command->tokens = tokens;
        return;
    }
    command->name = tokens[0];
    command->argc = parsed_count - 1;
    command->argv = (parsed_count > 1) ? tokens + 1 : NULL;
    command->tokens = tokens;
}

/**
 * @brief Creates a command structure from parsed tokens
 *
 * @param tokens Array of string tokens representing command components
 * @param parsed_count Number of tokens in the array
 * @return command_t* Pointer to newly created command structure, or NULL on
 * failure
 */
command_t *create_command_from_tokens(char **tokens, int parsed_count)
{
    command_t *command = init_command();

    if (command == NULL)
        return NULL;
    setup_command(command, tokens, parsed_count);
    return command;
}
