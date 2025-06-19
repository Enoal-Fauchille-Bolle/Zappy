#ifndef PLAYER_NEW_COMMAND_HPP
#define PLAYER_NEW_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerNewCommand : public ICommand {
public:
    PlayerNewCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_NEW_COMMAND_HPP
