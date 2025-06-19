#include "ServerBadParamCommand.hpp"
#include <iostream>

ServerBadParamCommand::ServerBadParamCommand() 
{
}

void ServerBadParamCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "ServerBadParamCommand " << args << std::endl;
}
