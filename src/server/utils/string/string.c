/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** String Utils
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

/**
 * @brief Concatenate two strings with dynamic memory allocation.
 *
 * This function reallocates memory for the destination string to accommodate
 * the source string and appends the source string to the destination.
 *
 * @param dest Pointer to the destination string
 * @param src Pointer to the source string
 * @return Pointer to the newly allocated concatenated string, or NULL on
 * failure
 */
char *dyn_strcat(char **original_dest, const char *src)
{
    char *dest = *original_dest;

    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Invalid string pointer\n");
        return NULL;
    }
    dest = realloc(dest, strlen(dest) + strlen(src) + 1);
    if (dest == NULL) {
        perror("Failed to reallocate memory for string concatenation");
        return NULL;
    }
    *original_dest = dest;
    strcat(dest, src);
    return dest;
}

/**
 * @brief Create an empty string with a null terminator.
 *
 * This function allocates memory for an empty string (a string with only
 * a null terminator) and returns a pointer to it. If memory allocation fails,
 * it prints an error message and returns NULL.
 *
 * @return Pointer to the newly allocated empty string, or NULL on failure
 */
char *empty_string(void)
{
    char *str = malloc(sizeof(char) * 1);

    if (str == NULL) {
        perror("Failed to allocate memory for empty string");
        return NULL;
    }
    str[0] = '\0';
    return str;
}
