#include "ServerSetTimeCommand.hpp"
#include <iostream>

ServerSetTimeCommand::ServerSetTimeCommand()
{
}

void ServerSetTimeCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "ServerSetTimeCommand " << args << std::endl;
}