#ifndef PLAYER_INCANTATION_COMMAND_HPP
#define PLAYER_INCANTATION_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerIncantationCommand : public ICommand {
public:
    PlayerIncantationCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_INCANTATION_COMMAND_HPP