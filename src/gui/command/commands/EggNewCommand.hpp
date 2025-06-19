#ifndef EGG_NEW_COMMAND_HPP
#define EGG_NEW_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class EggNewCommand : public ICommand {
public:
    EggNewCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // EGG_NEW_COMMAND_HPP
