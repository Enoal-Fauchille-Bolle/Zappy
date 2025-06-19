#include "PlayerGetCommand.hpp"
#include <iostream>

PlayerGetCommand::PlayerGetCommand()
{
}

void PlayerGetCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerGetCommand " << args << std::endl;
}