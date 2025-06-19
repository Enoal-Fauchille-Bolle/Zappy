#include "ServerUnknownCommand.hpp"
#include <iostream>

ServerUnknownCommand::ServerUnknownCommand()
{
}

void ServerUnknownCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "ServerUnknownCommand " << args << std::endl;
}