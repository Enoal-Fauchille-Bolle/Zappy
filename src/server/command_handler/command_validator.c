/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Validator
*/

#include "command_handler/command_validator.h"
#include "utils/string.h"
#include <string.h>

/**
 * @brief Validates if a buffer contains valid non-empty content
 *
 * This function checks if the provided buffer is not NULL, trims any
 * whitespace characters, and verifies that the buffer contains at least
 * one character after trimming.
 *
 * @param buffer The string buffer to validate
 * @return 1 if the buffer is valid and non-empty after trimming, 0 otherwise
 */
int validate_buffer(char *buffer)
{
    if (buffer == NULL)
        return 0;
    trim(buffer);
    if (strlen(buffer) == 0)
        return 0;
    return 1;
}
