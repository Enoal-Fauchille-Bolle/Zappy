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
    #include <time.h>
    #include <sys/time.h>
    #include <bits/types/struct_timeval.h>

    #define DEBUG_INFO "[ℹ️ ] "
    #define DEBUG_WARNING "[⚠️ ] "
    #define DEBUG_ERROR "[❌] "

static inline void debug_print(bool debug_enabled, const char *prefix,
    const char *format, va_list args)
{
    struct timeval tv;
    struct tm *tm_info;
    char timestamp[30];

    if (debug_enabled) {
        gettimeofday(&tv, NULL);
        tm_info = localtime(&tv.tv_sec);
        strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);
        printf("[%s.%03ld] %s", timestamp, tv.tv_usec / 1000, prefix);
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
