#ifndef PLAYER_DROP_COMMAND_HPP
#define PLAYER_DROP_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerDropCommand : public ICommand {
public:
    PlayerDropCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_DROP_COMMAND_HPP
