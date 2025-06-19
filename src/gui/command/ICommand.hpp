/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ICommand
*/

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>
#include "../GameManager.hpp"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(const std::string& args, SimpleGameManager& gameManager) = 0;
};

#endif // ICOMMAND_HPP
