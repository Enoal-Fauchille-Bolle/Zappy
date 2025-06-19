#include "EggDieCommand.hpp"
#include <iostream>

EggDieCommand::EggDieCommand()
{
}

void EggDieCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager; // Avoid unused parameter warning
    std::cout << "EggDieCommand " << args << std::endl;
}