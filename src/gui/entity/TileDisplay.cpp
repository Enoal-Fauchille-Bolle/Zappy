/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile display implementation
*/

#include "TileDisplay.hpp"

TileDisplay::TileDisplay(int x, int y) 
    : Entity(Utils::generateTileId(x, y), "cube.mesh"), 
      _x(x), _y(y), _mapWidth(10), _mapHeight(10) 
{
    _contentManager = new TileContentManager(x, y);
    setupTileAppearance();
}

TileDisplay::~TileDisplay() {
    delete _contentManager;
}

void TileDisplay::updateVisuals() {
    // Update tile appearance based on content
    // This can be extended for different visual states
}

void TileDisplay::setMapSize(int width, int height) {
    _mapWidth = width;
    _mapHeight = height;
}

TileContentManager* TileDisplay::getContentManager() const {
    return _contentManager;
}

std::pair<int, int> TileDisplay::getCoordinates() const {
    return std::make_pair(_x, _y);
}

void TileDisplay::setupTileAppearance() {
    setScale(Constants::TILE_SCALE, Constants::TILE_SCALE, Constants::TILE_SCALE);

    Color tileColor = getTileColor();
    setColor(tileColor);
}

Color TileDisplay::getTileColor() const {
    if ((_x + _y) % 2 == 0) {
        return Color(0.0f, 0.8f, 0.0f, 1.0f); // Light green
    } else {
        return Color(0.03f, 0.1f, 0.03f, 1.0f); // Dark green
    }
}

void TileDisplay::initialize() {
    setupTileAppearance();
}
