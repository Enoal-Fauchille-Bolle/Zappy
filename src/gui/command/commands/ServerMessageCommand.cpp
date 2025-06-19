#include "ServerMessageCommand.hpp"
#include <iostream>

ServerMessageCommand::ServerMessageCommand()
{
}

void ServerMessageCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "ServerMessageCommand " << args << std::endl;
}