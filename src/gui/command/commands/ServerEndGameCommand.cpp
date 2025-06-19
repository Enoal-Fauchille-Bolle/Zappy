#include "ServerEndGameCommand.hpp"
#include <iostream>

ServerEndGameCommand::ServerEndGameCommand()
{
}

void ServerEndGameCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "ServerEndGameCommand " << args << std::endl;
}