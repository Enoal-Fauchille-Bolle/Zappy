#include "PlayerNewCommand.hpp"
#include <iostream>

PlayerNewCommand::PlayerNewCommand()
{
}

void PlayerNewCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerNewCommand " << args << std::endl;
}