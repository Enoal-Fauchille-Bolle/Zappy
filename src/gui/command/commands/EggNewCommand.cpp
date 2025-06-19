#include "EggNewCommand.hpp"
#include <iostream>

EggNewCommand::EggNewCommand()
{
}

void EggNewCommand::execute(const std::string& args, SimpleGameManager gameManager) {
    (void)gameManager;
    std::cout << "EggNewCommand " << args << std::endl;
}
