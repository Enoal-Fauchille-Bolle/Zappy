#include "PlayerIncantationCommand.hpp"
#include <iostream>

PlayerIncantationCommand::PlayerIncantationCommand()
{
}

void PlayerIncantationCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "PlayerIncantationCommand " << args << std::endl;
}