#include "MapSizeCommand.hpp"

MapSizeCommand::MapSizeCommand()
{
}

void MapSizeCommand::execute(const std::string& args, SimpleGameManager gameManager)
{
    int width, height;
    std::istringstream iss(args);
    iss >> width >> height;
    gameManager.setMapSize(width, height);
}