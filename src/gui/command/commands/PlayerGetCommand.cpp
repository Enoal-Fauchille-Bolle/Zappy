#include "PlayerGetCommand.hpp"
#include <iostream>

PlayerGetCommand::PlayerGetCommand()
{
}

void PlayerGetCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerGetCommand " << args << std::endl;
}