/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Validator Tests
*/

#include "command_handler/command_validator.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

Test(command_validator, validate_buffer_null)
{
    int result = validate_buffer(NULL);
    cr_assert_eq(result, 0, "NULL buffer should return 0");
}

Test(command_validator, validate_buffer_empty)
{
    char buffer[] = "";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 0, "Empty buffer should return 0");
}

Test(command_validator, validate_buffer_whitespace_only)
{
    char buffer[] = "   \t\n  ";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 0, "Whitespace-only buffer should return 0");
}

Test(command_validator, validate_buffer_valid_command)
{
    char buffer[] = "dummy";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 1, "Valid command should return 1");
}

Test(command_validator, validate_buffer_with_leading_spaces)
{
    char buffer[] = "  dummy  ";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 1, "Command with spaces should return 1");
}

Test(command_validator, validate_buffer_with_newline)
{
    char buffer[] = "dummy\n";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 1, "Command with newline should return 1");
}

Test(command_validator, validate_buffer_mixed_whitespace)
{
    char buffer[] = "\t  dummy  \n";
    int result = validate_buffer(buffer);
    cr_assert_eq(result, 1, "Command with mixed whitespace should return 1");
}
