#include "BoardContentCommand.hpp"
#include <iostream>

BoardContentCommand::BoardContentCommand()
{
}

void BoardContentCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "BoardContentCommand " << args << std::endl;
}