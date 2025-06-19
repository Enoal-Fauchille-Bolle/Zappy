#include "BoardContentCommand.hpp"
#include <iostream>

BoardContentCommand::BoardContentCommand()
{
}

void BoardContentCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "BoardContentCommand " << args << std::endl;
}