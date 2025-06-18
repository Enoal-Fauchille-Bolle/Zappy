/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time Management
*/

#include "connection/server.h"
#include <bits/time.h>
#include <time.h>

/**
 * @brief Gets the current time in milliseconds since the epoch
 *
 * This function retrieves the current time using CLOCK_MONOTONIC and converts
 * it to milliseconds.
 *
 * @return long long Current time in milliseconds
 */
long long get_current_time_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}

/**
 * @brief Waits for the remaining time in the current game tick
 *
 * This function calculates how long to wait until the next game tick based on
 * the server's tick rate. It uses nanosleep to pause execution for the
 * calculated duration.
 *
 * @param server Pointer to the server structure containing game tick rate
 * @param tick_start_time The start time of the current tick in milliseconds
 */
void wait_remaining_tick_time(server_t *server, long long tick_start_time)
{
    long long elapsed = get_current_time_ms() - tick_start_time;
    long long tick_duration_ms = (1.0 / server->game->tick_rate) * 1000.0;
    long long remaining = 0;
    struct timespec sleep_time = {0};

    if (elapsed < tick_duration_ms) {
        remaining = tick_duration_ms - elapsed;
        sleep_time = (struct timespec){.tv_sec = remaining / 1000,
            .tv_nsec = (remaining % 1000) * 1000000};
        nanosleep(&sleep_time, NULL);
    }
}
