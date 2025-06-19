#ifndef TEAM_NAMES_COMMAND_HPP
#define TEAM_NAMES_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class TeamNamesCommand : public ICommand {
public:
    TeamNamesCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // TEAM_NAMES_COMMAND_HPP
