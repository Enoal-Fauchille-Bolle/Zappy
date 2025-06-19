#include "PlayerLevelCommand.hpp"
#include <iostream>

PlayerLevelCommand::PlayerLevelCommand()
{
}

void PlayerLevelCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "PlayerLevelCommand " << args << std::endl;
}
