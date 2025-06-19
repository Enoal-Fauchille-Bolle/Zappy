#ifndef PLAYER_EXPELLED_COMMAND_HPP
#define PLAYER_EXPELLED_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerExpelledCommand : public ICommand {
public:
    PlayerExpelledCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_EXPELLED_COMMAND_HPP