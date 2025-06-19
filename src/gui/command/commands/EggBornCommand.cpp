#include "EggBornCommand.hpp"
#include <iostream>

EggBornCommand::EggBornCommand()
{
}

void EggBornCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "EggBornCommand " << args << std::endl;
}