#ifndef PLAYER_DIE_COMMAND_HPP
#define PLAYER_DIE_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerDieCommand : public ICommand {
public:
    PlayerDieCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_DIE_COMMAND_HPP
