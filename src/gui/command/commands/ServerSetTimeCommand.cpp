/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerSetTimeCommand
*/

#include "ServerSetTimeCommand.hpp"
#include <iostream>

ServerSetTimeCommand::ServerSetTimeCommand()
{
}

void ServerSetTimeCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "ServerSetTimeCommand " << args << std::endl;
}
