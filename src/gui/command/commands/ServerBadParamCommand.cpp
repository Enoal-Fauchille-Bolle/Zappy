#include "ServerBadParamCommand.hpp"
#include <iostream>

ServerBadParamCommand::ServerBadParamCommand() 
{
}

void ServerBadParamCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "ServerBadParamCommand " << args << std::endl;
}