/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** String Utils
*/

#include <ctype.h>
#include <string.h>

/**
 * @brief Converts all characters in a string to lowercase
 *
 * This function modifies the input string in-place, converting each
 * lowercase character to its lowercase equivalent using the toupper()
 * function. Non-alphabetic characters remain unchanged.
 *
 * @param str Pointer to the null-terminated string to be converted.
 *            The string is modified in-place.
 *
 * @note The function assumes the input string is valid and null-terminated.
 * @note The string is modified directly; no copy is made.
 */
void to_lowercase(char *str)
{
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

/**
 * @brief Trim leading and trailing whitespace from a string.
 *
 * This function removes leading and trailing whitespace characters
 * (spaces, tabs, newlines) from the input string.
 *
 * @param str The string to trim.
 * @return char* The trimmed string.
 */
void trim(char *str)
{
    char *start = str;
    char *end = start + strlen(start) - 1;

    if (!str || *str == '\0')
        return;
    while (*start && isspace(*start))
        start++;
    if (*start == '\0') {
        *str = '\0';
        return;
    }
    while (end > start && isspace(*end)) {
        *end = '\0';
        end--;
    }
    if (start != str)
        memmove(str, start, strlen(start) + 1);
}
