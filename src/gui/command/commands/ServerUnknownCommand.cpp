/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerUnknownCommand
*/

#include "ServerUnknownCommand.hpp"
#include <iostream>

ServerUnknownCommand::ServerUnknownCommand()
{
}

void ServerUnknownCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "ServerUnknownCommand " << args << std::endl;
}
