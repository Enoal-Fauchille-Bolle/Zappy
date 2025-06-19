#include "ServerUnknownCommand.hpp"
#include <iostream>

ServerUnknownCommand::ServerUnknownCommand()
{
}

void ServerUnknownCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "ServerUnknownCommand " << args << std::endl;
}
