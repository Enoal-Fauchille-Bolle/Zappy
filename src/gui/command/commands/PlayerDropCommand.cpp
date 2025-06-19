#include "PlayerDropCommand.hpp"
#include <iostream>

PlayerDropCommand::PlayerDropCommand()
{
}

void PlayerDropCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "PlayerDropCommand " << args << std::endl;
}