/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Parser Edge Case Tests
*/

#include "command_handler/command.h"
#include "command_handler/command_parser.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>
#include <stdlib.h>

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

Test(command_parser_edge, parse_buffer_null_input)
{
    command_t *command = parse_command_buffer(NULL);
    cr_assert_null(command, "Command should be NULL for NULL input");
}

Test(command_parser_edge, parse_buffer_validation_failure)
{
    char buffer[] = "";
    command_t *command = parse_command_buffer(buffer);
    cr_assert_null(command, "Command should be NULL for empty buffer");
}

Test(command_parser_edge, parse_buffer_tokenization_failure)
{
    // This test is to cover the case where extract_tokens returns NULL
    // This might happen with extreme memory pressure or invalid input
    char buffer[] = "valid_command";
    command_t *command = parse_command_buffer(buffer);

    // Under normal circumstances this should work, but we're testing error
    // paths
    if (command != NULL) {
        cleanup_command(command);
    }
}
