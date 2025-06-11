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

#define DEBUG_INFO "[ℹ️ ] "
#define DEBUG_WARNING "[⚠️ ] "
#define DEBUG_ERROR "[❌] "
#define DEBUG_SERVER "[🖥️ ] "
#define DEBUG_CONN "[🔗] "
#define DEBUG_CMD "[⚡] "
#define DEBUG_GAME "[🎮] "
#define DEBUG_PLAYER "[👤] "
#define DEBUG_MAP "[🌐] "
#define DEBUG_RESOURCE "[💎] "

static inline void debug_print(bool debug_enabled, const char *prefix, const char *format, va_list args)
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

static inline void debug_server(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_SERVER, format, args);
    va_end(args);
}

static inline void debug_conn(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_CONN, format, args);
    va_end(args);
}

static inline void debug_cmd(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_CMD, format, args);
    va_end(args);
}

static inline void debug_game(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_GAME, format, args);
    va_end(args);
}

static inline void debug_player(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_PLAYER, format, args);
    va_end(args);
}

static inline void debug_map(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_MAP, format, args);
    va_end(args);
}

static inline void debug_resource(bool debug_enabled, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    debug_print(debug_enabled, DEBUG_RESOURCE, format, args);
    va_end(args);
}

#endif /* !DEBUG_H_ */
