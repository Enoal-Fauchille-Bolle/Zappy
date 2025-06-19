#include "PlayerExpelledCommand.hpp"
#include <iostream>

PlayerExpelledCommand::PlayerExpelledCommand()
{
}

void PlayerExpelledCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "PlayerExpelledCommand " << args << std::endl;
}