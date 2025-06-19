#include "PlayerBroadcastCommand.hpp"
#include <iostream>

PlayerBroadcastCommand::PlayerBroadcastCommand()
{
}

void PlayerBroadcastCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerBroadcastCommand " << args << std::endl;
}