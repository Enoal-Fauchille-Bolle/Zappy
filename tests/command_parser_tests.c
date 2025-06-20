/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Parser Tests
*/

#include "command_handler/command.h"
#include "command_handler/command_parser.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Helper function to properly clean up a command structure
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

Test(command_parser, parse_simple_command)
{
    char buffer[] = "dummy";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "dummy", "Command name should be 'dummy'");
    cr_assert_eq(command->argc, 0, "Command should have 0 arguments");
    cr_assert_null(command->argv, "argv should be NULL when argc is 0");

    cleanup_command(command);
}

Test(command_parser, parse_command_with_arguments)
{
    char buffer[] = "move forward 5";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "move", "Command name should be 'move'");
    cr_assert_eq(command->argc, 2, "Command should have 2 arguments");
    cr_assert_str_eq(
        command->argv[0], "forward", "First argument should be 'forward'");
    cr_assert_str_eq(command->argv[1], "5", "Second argument should be '5'");

    cleanup_command(command);
}

Test(command_parser, parse_command_with_newline)
{
    char buffer[] = "dummy\n";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "dummy", "Command name should be 'dummy'");
    cr_assert_eq(command->argc, 0, "Command should have 0 arguments");

    cleanup_command(command);
}

Test(command_parser, parse_command_with_trailing_spaces)
{
    char buffer[] = "  dummy  ";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "dummy", "Command name should be 'dummy'");
    cr_assert_eq(command->argc, 0, "Command should have 0 arguments");

    cleanup_command(command);
}

Test(command_parser, parse_empty_buffer)
{
    char buffer[] = "";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_null(command, "Command should be NULL for empty buffer");
}

Test(command_parser, parse_whitespace_only_buffer)
{
    char buffer[] = "   ";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_null(
        command, "Command should be NULL for whitespace-only buffer");
}

Test(command_parser, parse_command_with_multiple_spaces)
{
    char buffer[] = "command   arg1    arg2";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(
        command->name, "command", "Command name should be 'command'");
    cr_assert_eq(command->argc, 2, "Command should have 2 arguments");
    cr_assert_str_eq(
        command->argv[0], "arg1", "First argument should be 'arg1'");
    cr_assert_str_eq(
        command->argv[1], "arg2", "Second argument should be 'arg2'");

    cleanup_command(command);
}

Test(command_parser, parse_single_character_command)
{
    char buffer[] = "a";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "a", "Command name should be 'a'");
    cr_assert_eq(command->argc, 0, "Command should have 0 arguments");

    cleanup_command(command);
}

Test(command_parser, parse_command_with_many_arguments)
{
    char buffer[] = "test a b c d e f";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(command->name, "test", "Command name should be 'test'");
    cr_assert_eq(command->argc, 6, "Command should have 6 arguments");
    cr_assert_str_eq(command->argv[0], "a", "First argument should be 'a'");
    cr_assert_str_eq(command->argv[5], "f", "Last argument should be 'f'");

    cleanup_command(command);
}

Test(command_parser, parse_command_newline_and_spaces)
{
    char buffer[] = "  command arg1  \n";
    command_t *command = parse_command_buffer(buffer);

    cr_assert_not_null(command, "Command should not be NULL");
    cr_assert_str_eq(
        command->name, "command", "Command name should be 'command'");
    cr_assert_eq(command->argc, 1, "Command should have 1 argument");
    cr_assert_str_eq(
        command->argv[0], "arg1", "First argument should be 'arg1'");

    cleanup_command(command);
}

Test(command_parser, parse_null_buffer)
{
    command_t *command = parse_command_buffer(NULL);

    cr_assert_null(command, "Command should be NULL for NULL buffer");
}
