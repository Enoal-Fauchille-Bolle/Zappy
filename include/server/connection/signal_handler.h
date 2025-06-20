/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Signal Handler Header
*/

#ifndef SIGNAL_HANDLER_H_
    #define SIGNAL_HANDLER_H_

    #include <stdbool.h>

int setup_signal_handler(void);
bool handle_signal(int signal_fd, bool debug);

#endif /* !SIGNAL_HANDLER_H_ */
