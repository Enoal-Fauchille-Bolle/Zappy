#ifndef EGG_DIE_COMMAND_HPP
#define EGG_DIE_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class EggDieCommand : public ICommand {
public:
    EggDieCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // EGG_DIE_COMMAND_HPP