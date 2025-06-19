#include "EggBornCommand.hpp"
#include <iostream>

EggBornCommand::EggBornCommand()
{
}

void EggBornCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "EggBornCommand " << args << std::endl;
}