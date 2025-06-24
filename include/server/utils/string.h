/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** String Utils Header
*/
// clang-format Language: C

#ifndef STRING_H_
    #define STRING_H_
    #include <stddef.h>
    #include <sys/types.h>

void to_lowercase(char *str);
void trim(char *str);
char *dyn_strcat(char **original_dest, const char *src);
char *empty_string(size_t size);
char *repeat_string(const char *str, const char *delim, uint count);

#endif /* !STRING_H_ */
