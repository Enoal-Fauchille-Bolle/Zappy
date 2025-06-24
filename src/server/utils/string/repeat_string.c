/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** repeat_string
*/

#include "utils/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Repeat a string a specified number of times with a delimiter.
 *
 * This function creates a new string that consists of the input string
 * repeated 'count' times, separated by the specified delimiter.
 *
 * @param str The string to repeat.
 * @param delim The delimiter to insert between repetitions.
 * @param count The number of times to repeat the string.
 * @return A newly allocated string containing the repeated pattern,
 *         or NULL if memory allocation fails.
 */
char *repeat_string(const char *str, const char *delim, uint count)
{
    size_t delim_len = delim ? strlen(delim) : 0;
    ssize_t total_len =
        count && str ? (strlen(str) + delim_len) * count - delim_len : 0;
    char *result = empty_string(total_len);

    if (!result)
        return NULL;
    for (size_t i = 0; i < count; i++) {
        strcat(result, str);
        if (i < count - 1)
            strcat(result, delim);
    }
    return result;
}
