#include "PlayerIncantationEndCommand.hpp"
#include <iostream>

PlayerIncantationEndCommand::PlayerIncantationEndCommand()
{
}

void PlayerIncantationEndCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerIncantationEndCommand " << args << std::endl;
}