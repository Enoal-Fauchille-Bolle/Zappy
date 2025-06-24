/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time Management Header
*/

#ifndef TIME_H_
    #define TIME_H_

    #include "connection/server.h"

long long get_current_time_ms(void);
void wait_remaining_tick_time(
    server_t *server, long long tick_start_time);

#endif /* !TIME_H_ */
