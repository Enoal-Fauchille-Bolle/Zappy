#ifndef PLAYER_FORK_COMMAND_HPP
#define PLAYER_FORK_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerForkCommand : public ICommand {
public:
    PlayerForkCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_FORK_COMMAND_HPP
