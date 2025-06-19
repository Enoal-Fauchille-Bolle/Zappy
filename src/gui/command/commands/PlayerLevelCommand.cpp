#include "PlayerLevelCommand.hpp"
#include <iostream>

PlayerLevelCommand::PlayerLevelCommand()
{
}

void PlayerLevelCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerLevelCommand " << args << std::endl;
}