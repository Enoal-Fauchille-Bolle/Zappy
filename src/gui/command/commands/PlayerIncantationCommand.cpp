#include "PlayerIncantationCommand.hpp"
#include <iostream>

PlayerIncantationCommand::PlayerIncantationCommand()
{
}

void PlayerIncantationCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerIncantationCommand " << args << std::endl;
}