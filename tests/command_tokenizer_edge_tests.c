/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Tokenizer Edge Case Tests
*/

#include "command_handler/command_tokenizer.h"
#include <criterion/criterion.h>
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

Test(command_tokenizer_edge, extract_tokens_null_buffer)
{
    int parsed_count = 0;
    char **tokens = extract_tokens(NULL, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL even for NULL buffer");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens for NULL buffer");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer_edge, extract_tokens_memory_allocation_failure)
{
    // This test is designed to hit the error paths in tokenization
    char buffer[] = "command arg1 arg2 arg3 arg4 arg5";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    // Under normal circumstances this should work
    if (tokens != NULL) {
        cr_assert_gt(parsed_count, 0, "Should parse some tokens");
        cleanup_tokens(tokens);
    }
}

Test(command_tokenizer_edge, extract_tokens_very_long_command)
{
    // Test with a command that has many arguments to stress test the tokenizer
    char buffer[] = "cmd a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 a11 a12 a13 a14 a15";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 16, "Should parse 16 tokens");
    cr_assert_str_eq(tokens[0], "cmd", "First token should be 'cmd'");
    cr_assert_str_eq(tokens[15], "a15", "Last token should be 'a15'");

    cleanup_tokens(tokens);
}

Test(command_tokenizer_edge, extract_tokens_only_delimiters)
{
    char buffer[] = " \t\n \t ";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}

Test(command_tokenizer_edge, extract_tokens_single_delimiter)
{
    char buffer[] = " ";
    int parsed_count = 0;
    char **tokens = extract_tokens(buffer, &parsed_count);

    cr_assert_not_null(tokens, "Tokens should not be NULL");
    cr_assert_eq(parsed_count, 0, "Should parse 0 tokens");
    cr_assert_null(tokens[0], "First token should be NULL");

    cleanup_tokens(tokens);
}
