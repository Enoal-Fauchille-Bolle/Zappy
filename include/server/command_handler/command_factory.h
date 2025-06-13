/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Factory Header
*/

#ifndef COMMAND_FACTORY_H_
    #define COMMAND_FACTORY_H_

    #include "command_handler/command.h"

command_t *create_command_from_tokens(char **tokens, int parsed_count);

#endif /* !COMMAND_FACTORY_H_ */
