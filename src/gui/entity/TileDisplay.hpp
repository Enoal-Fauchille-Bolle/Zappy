/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile display - handles visual representation of tiles
*/

#ifndef TILE_DISPLAY_HPP_
#define TILE_DISPLAY_HPP_

#include "Entity.hpp"
#include "TileContentManager.hpp"
#include "../Types.hpp"

class TileDisplay : public Entity {
public:
    TileDisplay(int x, int y);
    ~TileDisplay();

    void initialize() override;

    void updateVisuals();
    void setMapSize(int width, int height);

    TileContentManager* getContentManager() const;
    std::pair<int, int> getCoordinates() const;
    void setupTileAppearance();

private:
    int _x, _y;
    int _mapWidth, _mapHeight;
    TileContentManager* _contentManager;

    Color getTileColor() const;
};

#endif /* !TILE_DISPLAY_HPP_ */
