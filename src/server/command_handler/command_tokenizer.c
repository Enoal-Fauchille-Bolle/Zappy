/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Tokenizer
*/

#include "command_handler/command_tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Processes a single character to count words in a buffer
 *
 * This function analyzes a character at a specific position in a buffer to
 * determine word boundaries and increment the word count accordingly. Words
 * are delimited by spaces, tabs, and newlines.
 *
 * @param buffer The input string buffer to process
 * @param i The index of the character to process in the buffer
 * @param count Pointer to the word counter (incremented when a new word
 * starts)
 * @param in_word Pointer to boolean flag tracking if currently inside a word
 */
static void process_character(char *buffer, int i, int *count, bool *in_word)
{
    if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n') {
        if (!*in_word) {
            (*count)++;
            *in_word = true;
        }
    } else {
        *in_word = false;
    }
}

/**
 * @brief Count the number of arguments in a buffer.
 *
 * This function counts the number of space-separated arguments in a
 * given buffer by counting words rather than spaces.
 *
 * @param buffer The input buffer containing the command line.
 * @return int The number of arguments in the buffer.
 */
static int count_args(char *buffer)
{
    int count = 0;
    bool in_word = false;

    for (int i = 0; buffer[i] != '\0'; i++) {
        process_character(buffer, i, &count, &in_word);
    }
    return count;
}

/**
 * @brief Cleans up memory allocated for an array of string tokens
 *
 * Frees each individual token string and then frees the array itself.
 *
 * @param tokens Array of string pointers to be freed
 * @param count Number of tokens in the array
 */
static void cleanup_tokens(char **tokens, int count)
{
    for (int j = 0; j < count; j++)
        free(tokens[j]);
    free(tokens);
}

/**
 * @brief Remove trailing newline from the last token if it exists.
 *
 * This function checks if the last token contains a trailing newline
 * character and removes it by replacing it with a null terminator.
 *
 * @param tokens The array of tokens.
 * @param count The number of tokens processed.
 */
static void remove_trailing_newline(char **tokens, int count)
{
    size_t len = 0;

    if (count > 0 && tokens[count - 1] != NULL) {
        len = strlen(tokens[count - 1]);
        if (len > 0 && tokens[count - 1][len - 1] == '\n') {
            tokens[count - 1][len - 1] = '\0';
        }
    }
}

/**
 * @brief Parse tokens from buffer using strtok and duplicate them.
 *
 * This function uses strtok to split the buffer by spaces and creates
 * duplicated strings for each token.
 *
 * @param buffer The input buffer to parse.
 * @param tokens The array to store the duplicated tokens.
 * @param token_count The maximum number of tokens to parse.
 * @return int The number of tokens successfully parsed, or -1 on error.
 */
static int parse_tokens(char *buffer, char **tokens, int token_count)
{
    char *token = strtok(buffer, " \t\n");
    int i = 0;

    for (i = 0; token != NULL && i < token_count; i++) {
        tokens[i] = strdup(token);
        if (tokens[i] == NULL) {
            perror("strdup");
            cleanup_tokens(tokens, i);
            return -1;
        }
        token = strtok(NULL, " \t\n");
    }
    return i;
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
    int parsed_count = 0;

    if (tokens == NULL) {
        perror("malloc");
        return NULL;
    }
    for (int i = 0; i <= token_count; i++) {
        tokens[i] = NULL;
    }
    if (buffer == NULL || token_count == 0) {
        return tokens;
    }
    parsed_count = parse_tokens(buffer, tokens, token_count);
    if (parsed_count == -1) {
        free(tokens);
        return NULL;
    }
    remove_trailing_newline(tokens, parsed_count);
    return tokens;
}

/**
 * @brief Handle a null buffer case by returning an empty token array.
 *
 * This function allocates an array for tokens and sets the parsed count
 * to zero, indicating no tokens were parsed.
 *
 * @param parsed_count Pointer to store the number of tokens parsed.
 * @return char** Pointer to an array containing a single NULL token.
 */
static char **handle_null_buffer(int *parsed_count)
{
    char **tokens = malloc(sizeof(char *));

    *parsed_count = 0;
    if (tokens != NULL) {
        tokens[0] = NULL;
    }
    return tokens;
}

/**
 * @brief Count the number of valid tokens in an array.
 *
 * This function counts the number of non-NULL tokens in the provided
 * array of tokens, up to the expected count.
 *
 * @param tokens The array of tokens to count.
 * @param expected_count The expected number of tokens.
 * @return int The number of valid tokens found.
 */
static int count_valid_tokens(char **tokens, int expected_count)
{
    int i;

    for (i = 0; i < expected_count && tokens[i] != NULL; i++)
        ;
    return i;
}

/**
 * @brief Extract tokens from a command buffer.
 *
 * This function processes a command buffer, counts the number of tokens,
 * tokenizes the buffer, and returns an array of tokens.
 *
 * @param buffer The input command buffer to tokenize.
 * @param parsed_count Pointer to store the number of valid tokens parsed.
 * @return char** Pointer to an array of tokens, or NULL on failure.
 */
char **extract_tokens(char *buffer, int *parsed_count)
{
    int token_count;
    char **tokens;

    if (parsed_count == NULL) {
        return NULL;
    }
    if (buffer == NULL) {
        return handle_null_buffer(parsed_count);
    }
    token_count = count_args(buffer);
    tokens = tokenize_buffer(buffer, token_count);
    if (tokens == NULL) {
        *parsed_count = 0;
        return NULL;
    }
    *parsed_count = count_valid_tokens(tokens, token_count);
    return tokens;
}
