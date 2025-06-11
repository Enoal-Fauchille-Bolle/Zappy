/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Debug Header
*/

#ifndef DEBUG_H_
    #define DEBUG_H_

    #include <stdarg.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include "debug_categories.h"

    #define DEBUG_INFO "[ℹ️ ] "
    #define DEBUG_WARNING "[⚠️ ] "
    #define DEBUG_ERROR "[❌] "

static inline void debug_print(bool debug_enabled, const char *prefix,
    const char *format, va_list args)
{
    if (debug_enabled) {
        printf("%s", prefix);
        vprintf(format, args);
    }
}

static inline void debug_info(bool debug_enabled, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    debug_print(debug_enabled, DEBUG_INFO, format, args);
    va_end(args);
}

static inline void debug_warning(bool debug_enabled, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    debug_print(debug_enabled, DEBUG_WARNING, format, args);
    va_end(args);
}

static inline void debug_error(bool debug_enabled, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    debug_print(debug_enabled, DEBUG_ERROR, format, args);
    va_end(args);
}

#endif /* !DEBUG_H_ */
