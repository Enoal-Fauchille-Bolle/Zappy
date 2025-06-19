#include "PlayerDieCommand.hpp"
#include <iostream>

PlayerDieCommand::PlayerDieCommand()
{
}

void PlayerDieCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerDieCommand " << args << std::endl;
}