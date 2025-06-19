#include "ServerEndGameCommand.hpp"
#include <iostream>

ServerEndGameCommand::ServerEndGameCommand()
{
}

void ServerEndGameCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "ServerEndGameCommand " << args << std::endl;
}
