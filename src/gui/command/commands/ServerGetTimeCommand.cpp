#include "ServerGetTimeCommand.hpp"
#include <iostream>

ServerGetTimeCommand::ServerGetTimeCommand()
{
}

void ServerGetTimeCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "ServerGetTimeCommand " << args << std::endl;
}