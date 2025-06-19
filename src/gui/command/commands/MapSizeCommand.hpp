#ifndef MAP_SIZE_COMMAND_HPP
#define MAP_SIZE_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class MapSizeCommand : public ICommand {
public:
    MapSizeCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // MAP_SIZE_COMMAND_HPP