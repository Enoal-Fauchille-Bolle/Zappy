/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile display implementation
*/

#include "TileDisplay.hpp"

/**
 * @brief Constructor for TileDisplay.
 * 
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 */
TileDisplay::TileDisplay(int x, int y) 
    : Entity(Utils::generateTileId(x, y), "cube.mesh"), 
      _x(x), _y(y), _mapWidth(10), _mapHeight(10) 
{
    _contentManager = new TileContentManager(x, y);
    setupTileAppearance();
}

/**
 * @brief Destructor for TileDisplay.
 */
TileDisplay::~TileDisplay() {
    delete _contentManager;
}

/**
 * @brief Update the visual appearance of the tile.
 */
void TileDisplay::updateVisuals() {
}

/**
 * @brief Set the dimensions of the map.
 * 
 * @param width Width of the map.
 * @param height Height of the map.
 */
void TileDisplay::setMapSize(int width, int height) {
    _mapWidth = width;
    _mapHeight = height;
}

/**
 * @brief Get the content manager for this tile.
 * 
 * @return Pointer to the tile's content manager.
 */
TileContentManager* TileDisplay::getContentManager() const {
    return _contentManager;
}

/**
 * @brief Get the coordinates of this tile.
 * 
 * @return Pair containing the x and y coordinates.
 */
std::pair<int, int> TileDisplay::getCoordinates() const {
    return std::make_pair(_x, _y);
}

/**
 * @brief Set up the visual appearance of the tile.
 */
void TileDisplay::setupTileAppearance() {
    setScale(Constants::TILE_SCALE, Constants::TILE_SCALE, Constants::TILE_SCALE);

    Color tileColor = getTileColor();
    setColor(tileColor);
}

/**
 * @brief Get the color for this tile based on its position.
 * 
 * @return Color for the tile (checkerboard pattern).
 */
Color TileDisplay::getTileColor() const {
    if ((_x + _y) % 2 == 0) {
        return Color(0.0f, 0.8f, 0.0f, 1.0f);
    } else {
        return Color(0.03f, 0.1f, 0.03f, 1.0f);
    }
}

/**
 * @brief Initialize the tile display after being attached to the scene.
 */
void TileDisplay::initialize() {
    setupTileAppearance();
}
