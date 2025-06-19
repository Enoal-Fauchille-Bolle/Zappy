/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerGetTimeCommand
*/

#include "ServerGetTimeCommand.hpp"
#include <iostream>

ServerGetTimeCommand::ServerGetTimeCommand()
{
}

void ServerGetTimeCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "ServerGetTimeCommand " << args << std::endl;
}
