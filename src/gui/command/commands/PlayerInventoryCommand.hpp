#ifndef PLAYER_INVENTORY_COMMAND_HPP
#define PLAYER_INVENTORY_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerInventoryCommand : public ICommand {
public:
    PlayerInventoryCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_INVENTORY_COMMAND_HPP