/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Tokenizer Tests
*/

#include "command_handler/command_tokenizer.h"
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Helper function to clean up token arrays
 *
 * @param tokens Array of string tokens to free
 */
static void cleanup_tokens(char **tokens)
{
    if (tokens == NULL)
        return;

    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

Test(command_tokenizer, extract_tokens_simple_command)
{
    char buffer[] = "dummy";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 1, "Should parse 1 token");
    cr_assert_str_eq(tokens[0], "dummy", "First token should be 'dummy'");
    cr_assert_null(tokens[1], "Second token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_with_arguments)
{
    char buffer[] = "move forward 5";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 3, "Should parse 3 tokens");
    cr_assert_str_eq(tokens[0], "move", "First token should be 'move'");
    cr_assert_str_eq(tokens[1], "forward", "Second token should be 'forward'");
    cr_assert_str_eq(tokens[2], "5", "Third token should be '5'");
    cr_assert_null(tokens[3], "Fourth token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_multiple_spaces)
{
    char buffer[] = "command   arg1    arg2";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 3, "Should parse 3 tokens");
    cr_assert_str_eq(tokens[0], "command", "First token should be 'command'");
    cr_assert_str_eq(tokens[1], "arg1", "Second token should be 'arg1'");
    cr_assert_str_eq(tokens[2], "arg2", "Third token should be 'arg2'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_with_newline)
{
    char buffer[] = "dummy\n";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 1, "Should parse 1 token");
    cr_assert_str_eq(
        tokens[0], "dummy", "Token should be 'dummy' without newline");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_with_tabs)
{
    char buffer[] = "cmd\targ1\targ2";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 3, "Should parse 3 tokens");
    cr_assert_str_eq(tokens[0], "cmd", "First token should be 'cmd'");
    cr_assert_str_eq(tokens[1], "arg1", "Second token should be 'arg1'");
    cr_assert_str_eq(tokens[2], "arg2", "Third token should be 'arg2'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_mixed_whitespace)
{
    char buffer[] = " \t cmd \t arg1 \n arg2 \t ";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 3, "Should parse 3 tokens");
    cr_assert_str_eq(tokens[0], "cmd", "First token should be 'cmd'");
    cr_assert_str_eq(tokens[1], "arg1", "Second token should be 'arg1'");
    cr_assert_str_eq(tokens[2], "arg2", "Third token should be 'arg2'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_single_character)
{
    char buffer[] = "a";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 1, "Should parse 1 token");
    cr_assert_str_eq(tokens[0], "a", "Token should be 'a'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_many_arguments)
{
    char buffer[] = "test a b c d e f g h i j";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 11, "Should parse 11 tokens");
    cr_assert_str_eq(tokens[0], "test", "First token should be 'test'");
    cr_assert_str_eq(tokens[10], "j", "Last token should be 'j'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_empty_string)
{
    char buffer[] = "";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_whitespace_only)
{
    char buffer[] = "   \t\n  ";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_leading_trailing_spaces)
{
    char buffer[] = "  command arg  ";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 2, "Should parse 2 tokens");
    cr_assert_str_eq(tokens[0], "command", "First token should be 'command'");
    cr_assert_str_eq(tokens[1], "arg", "Second token should be 'arg'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_newline_at_end)
{
    char buffer[] = "command arg1 arg2\n";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 3, "Should parse 3 tokens");
    cr_assert_str_eq(tokens[0], "command", "First token should be 'command'");
    cr_assert_str_eq(tokens[1], "arg1", "Second token should be 'arg1'");
    cr_assert_str_eq(
        tokens[2], "arg2", "Third token should be 'arg2' without newline");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_single_space_separated)
{
    char buffer[] = "a b c d e";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 5, "Should parse 5 tokens");
    cr_assert_str_eq(tokens[0], "a", "First token should be 'a'");
    cr_assert_str_eq(tokens[4], "e", "Last token should be 'e'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_consecutive_spaces)
{
    char buffer[] = "cmd     arg";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 2, "Should parse 2 tokens");
    cr_assert_str_eq(tokens[0], "cmd", "First token should be 'cmd'");
    cr_assert_str_eq(tokens[1], "arg", "Second token should be 'arg'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_null_buffer)
{
    char *buffer = NULL;
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer, extract_tokens_null_parsed_count)
{
    char buffer[] = "test";
    char **tokens = extract_tokens(buffer, NULL);

    cr_assert_null(tokens, "Tokens should be NULL when parsed_count is NULL");

    cleanup_tokens(tokens);
}
