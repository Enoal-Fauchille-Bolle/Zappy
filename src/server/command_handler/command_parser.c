/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Parser
*/

#include "command_handler/command_parser.h"
#include "command_handler/command.h"
#include "utils/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    command->argc = 0;
    command->argv = NULL;
    return command;
}

/**
 * @brief Count the number of arguments in a buffer.
 *
 * This function counts the number of space-separated arguments in a
 * given buffer.
 *
 * @param buffer The input buffer containing the command line.
 * @return int The number of arguments in the buffer.
 */
static int count_args(char *buffer)
{
    int count = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ')
            count++;
    }
    return count + 1;
}

/**
 * @brief Tokenize a buffer into an array of strings.
 *
 * This function splits a buffer into tokens based on spaces and
 * returns an array of strings.
 *
 * @param buffer The input buffer to tokenize.
 * @param token_count The number of tokens to create.
 * @return char** Pointer to the array of tokens.
 */
static char **tokenize_buffer(char *buffer, int token_count)
{
    char **tokens = malloc(sizeof(char *) * (token_count + 1));
    char *token = NULL;

    if (tokens == NULL) {
        perror("malloc");
        return NULL;
    }
    token = strtok(buffer, " ");
    for (int i = 0; token != NULL; i++) {
        tokens[i] = strdup(token);
        token = strtok(NULL, " ");
    }
    if (tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] == '\n')
        tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] = '\0';
    tokens[token_count] = NULL;
    return tokens;
}

/**
 * @brief Set up the command structure with the parsed tokens.
 *
 * This function initializes the command structure with the command name,
 * argument count, and argument values.
 *
 * @param command The command structure to set up.
 * @param tokens The array of tokens parsed from the buffer.
 * @param token_count The number of tokens in the array.
 */
static void setup_command(command_t *command, char **tokens, int token_count)
{
    command->argc = token_count - 1;
    command->name = tokens[0];
    command->argv = tokens + 1;
}

/**
 * @brief Parse a buffer into a command structure.
 *
 * This function takes a buffer containing a command line, parses it,
 * and returns a command structure with the command name and arguments.
 *
 * @param buffer The input buffer containing the command line.
 * @return command_t* Pointer to the parsed command structure.
 */
command_t *parse_command_buffer(char *buffer)
{
    command_t *command = init_command();
    int token_count = 0;
    char **tokens = NULL;

    if (command == NULL)
        return NULL;
    trim(buffer);
    if (strlen(buffer) == 0) {
        free(command);
        return NULL;
    }
    token_count = count_args(buffer);
    tokens = tokenize_buffer(buffer, token_count);
    if (tokens == NULL) {
        free(command);
        return NULL;
    }
    setup_command(command, tokens, token_count);
    return command;
}
