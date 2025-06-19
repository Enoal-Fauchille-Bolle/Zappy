#include "BoardContentCommand.hpp"
#include <iostream>

BoardContentCommand::BoardContentCommand()
{
}

void BoardContentCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "BoardContentCommand " << args << std::endl;
}
