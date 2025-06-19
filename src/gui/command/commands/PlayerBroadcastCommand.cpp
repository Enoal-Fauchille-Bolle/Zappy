#include "PlayerBroadcastCommand.hpp"
#include <iostream>

PlayerBroadcastCommand::PlayerBroadcastCommand()
{
}

void PlayerBroadcastCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerBroadcastCommand " << args << std::endl;
}