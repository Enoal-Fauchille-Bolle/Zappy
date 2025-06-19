/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Factory Tests
*/

#include "command_handler/command.h"
#include "command_handler/command_factory.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function to clean up command structures
 *
 * @param command The command structure to clean up
 */
static void cleanup_command(command_t *command)
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
}

/**
 * @brief Helper function to create test tokens
 *
 * @param token_strings Array of strings to convert to tokens
 * @param count Number of tokens
 * @return char** Array of duplicated token strings
 */
static char **create_test_tokens(const char **token_strings, int count)
{
    char **tokens = malloc(sizeof(char *) * (count + 1));

    if (tokens == NULL)
        return NULL;

    for (int i = 0; i < count; i++) {
        tokens[i] = strdup(token_strings[i]);
        if (tokens[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(tokens);
            return NULL;
        }
    }
    tokens[count] = NULL;
    return tokens;
}

Test(command_factory, create_command_simple)
{
    const char *token_strings[] = {"dummy"};
    char **tokens = create_test_tokens(token_strings, 1);
    command_t *command = create_command_from_tokens(tokens, 1);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "dummy", "Command name should be 'dummy'");
    cr_assert_eq(command->argc, 0, "Command should have 0 arguments");
    cr_assert_null(command->argv, "argv should be NULL when argc is 0");
    cr_assert_not_null(command->tokens, "tokens should not be NULL");

    cleanup_command(command);
}

Test(command_factory, create_command_with_arguments)
{
    const char *token_strings[] = {"move", "forward", "5"};
    char **tokens = create_test_tokens(token_strings, 3);
    command_t *command = create_command_from_tokens(tokens, 3);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "move", "Command name should be 'move'");
    cr_assert_eq(command->argc, 2, "Command should have 2 arguments");
    cr_assert_not_null(command->argv, "argv should not be NULL");
    cr_assert_str_eq(
        command->argv[0], "forward", "First argument should be 'forward'");
    cr_assert_str_eq(command->argv[1], "5", "Second argument should be '5'");

    cleanup_command(command);
}

Test(command_factory, create_command_with_many_arguments)
{
    const char *token_strings[] = {"test", "a", "b", "c", "d", "e"};
    char **tokens = create_test_tokens(token_strings, 6);
    command_t *command = create_command_from_tokens(tokens, 6);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "test", "Command name should be 'test'");
    cr_assert_eq(command->argc, 5, "Command should have 5 arguments");
    cr_assert_str_eq(command->argv[0], "a", "First argument should be 'a'");
    cr_assert_str_eq(command->argv[4], "e", "Last argument should be 'e'");

    cleanup_command(command);
}

Test(command_factory, create_command_memory_failure)
{
    // This test simulates memory allocation failure by passing NULL tokens
    command_t *command = create_command_from_tokens(NULL, 0);

    cr_assert_not_null(
        command, "Command should not be NULL even with NULL tokens");
    cr_assert_null(command->name, "Command name should be NULL");
    cr_assert_eq(command->argc, -1, "Command should have -1 arguments");
    cr_assert_null(command->argv, "argv should be NULL");
    cr_assert_null(command->tokens, "tokens should be NULL");

    free(command);
}

Test(command_factory, create_command_zero_tokens)
{
    char **tokens = malloc(sizeof(char *));
    tokens[0] = NULL;
    command_t *command = create_command_from_tokens(tokens, 0);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_eq(command->argc, -1, "Command should have -1 arguments");
    cr_assert_null(command->argv, "argv should be NULL");

    free(tokens);
    free(command);
}
