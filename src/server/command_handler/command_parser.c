/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Parser
*/

#include "command_handler/command_parser.h"
#include "command_handler/command.h"
#include "command_handler/command_factory.h"
#include "command_handler/command_tokenizer.h"
#include "command_handler/command_validator.h"
#include <stddef.h>

/**
 * @brief Parses a command buffer and creates a command structure
 *
 * This function takes a raw command buffer string, validates it, extracts
 * tokens from it, and creates a command structure based on those tokens.
 *
 * @param buffer The input command buffer string to parse
 * @return command_t* Pointer to the created command structure, or NULL on
 * failure
 */
command_t *parse_command_buffer(char *buffer)
{
    char **tokens = NULL;
    int parsed_count = 0;
    command_t *command = NULL;

    if (!validate_buffer(buffer))
        return NULL;
    tokens = extract_tokens(buffer, &parsed_count);
    if (tokens == NULL)
        return NULL;
    command = create_command_from_tokens(tokens, parsed_count);
    return command;
}
