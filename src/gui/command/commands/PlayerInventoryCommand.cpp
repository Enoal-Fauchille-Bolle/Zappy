#include "PlayerInventoryCommand.hpp"
#include <iostream>

PlayerInventoryCommand::PlayerInventoryCommand()
{
}

void PlayerInventoryCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerInventoryCommand " << args << std::endl;
}