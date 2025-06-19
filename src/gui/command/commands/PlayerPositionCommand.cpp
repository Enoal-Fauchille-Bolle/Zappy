#include "PlayerPositionCommand.hpp"
#include <iostream>

PlayerPositionCommand::PlayerPositionCommand()
{
}

void PlayerPositionCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerPositionCommand " << args << std::endl;
}