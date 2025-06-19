#include "TeamNamesCommand.hpp"
#include <iostream>

TeamNamesCommand::TeamNamesCommand()
{
}

void TeamNamesCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "TeamNamesCommand " << args << std::endl;
}