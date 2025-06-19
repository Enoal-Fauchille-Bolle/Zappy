#include "EggDieCommand.hpp"
#include <iostream>

EggDieCommand::EggDieCommand()
{
}

void EggDieCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    gameManager = gameManager;
    std::cout << "EggDieCommand " << args << std::endl;
}