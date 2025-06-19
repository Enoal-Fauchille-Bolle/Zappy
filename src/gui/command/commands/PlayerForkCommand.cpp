#include "PlayerForkCommand.hpp"
#include <iostream>

PlayerForkCommand::PlayerForkCommand()
{
}

void PlayerForkCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerForkCommand " << args << std::endl;
}