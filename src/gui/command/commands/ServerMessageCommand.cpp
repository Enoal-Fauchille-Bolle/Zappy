/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerMessageCommand
*/

#include "ServerMessageCommand.hpp"
#include <iostream>

ServerMessageCommand::ServerMessageCommand()
{
}

void ServerMessageCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "ServerMessageCommand " << args << std::endl;
}
