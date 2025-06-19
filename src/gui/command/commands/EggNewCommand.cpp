#include "EggNewCommand.hpp"
#include <iostream>

EggNewCommand::EggNewCommand()
{
}

void EggNewCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "EggNewCommand " << args << std::endl;
}