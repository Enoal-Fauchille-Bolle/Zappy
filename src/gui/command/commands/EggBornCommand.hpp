#ifndef EGG_BORN_COMMAND_HPP
#define EGG_BORN_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class EggBornCommand : public ICommand {
public:
    EggBornCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // EGG_BORN_COMMAND_HPP
