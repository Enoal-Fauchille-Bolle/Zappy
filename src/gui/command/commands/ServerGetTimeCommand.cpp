#include "ServerGetTimeCommand.hpp"
#include <iostream>

ServerGetTimeCommand::ServerGetTimeCommand()
{
}

void ServerGetTimeCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "ServerGetTimeCommand " << args << std::endl;
}