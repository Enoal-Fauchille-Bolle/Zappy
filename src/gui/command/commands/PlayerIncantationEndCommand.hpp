#ifndef PLAYER_INCANTATION_END_COMMAND_HPP
#define PLAYER_INCANTATION_END_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerIncantationEndCommand : public ICommand {
public:
    PlayerIncantationEndCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_INCANTATION_END_COMMAND_HPP