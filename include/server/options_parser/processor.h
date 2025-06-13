/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Options
*/

#ifndef OPTIONS_PROCESSOR_H_
    #define OPTIONS_PROCESSOR_H_

    #include <stdbool.h>
    #include <stddef.h>
    #include "options_parser/options.h"

bool process_options(server_options_t *options);
int handle_options_error(void);

#endif /* !OPTIONS_PROCESSOR_H_ */
