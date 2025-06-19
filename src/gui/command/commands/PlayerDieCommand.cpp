#include "PlayerDieCommand.hpp"
#include <iostream>

PlayerDieCommand::PlayerDieCommand()
{
}

void PlayerDieCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "PlayerDieCommand " << args << std::endl;
}
