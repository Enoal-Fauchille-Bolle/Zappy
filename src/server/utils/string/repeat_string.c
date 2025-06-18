/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** repeat_string
*/

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
char *repeat_string(const char *str, const char *delim, size_t count)
{
    size_t delim_len = strlen(delim);
    long total_len = (strlen(str) + delim_len) * count - delim_len;
    char *result;

    if (total_len < 0)
        total_len = 0;
    result = malloc(sizeof(char) * (total_len + 1));
    if (result == NULL) {
        perror("Failed to allocate memory for repeated string");
        return NULL;
    }
    result[0] = '\0';
    for (size_t i = 0; i < count; i++) {
        strcat(result, str);
        if (i < count - 1)
            strcat(result, delim);
    }
    return result;
}
