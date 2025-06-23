##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## vision
##

from typing import Optional, TypeAlias
from .utils import GameConstants, MessageParser

Tile: TypeAlias = list[str]

RelativePos: TypeAlias = tuple[int, int]
VisionIndex: TypeAlias = int
TilePos: TypeAlias = tuple[RelativePos, VisionIndex] # tuple[tuple[int, int], int]

ResourceName: TypeAlias = str
ResourcePos: TypeAlias = tuple[ResourceName, RelativePos, VisionIndex]

class PlayerVision:
    """Manage the player vison aka world state"""

    def __init__(self) -> None:
        self.width: int = 0
        self.height: int = 0
        self.player_pos: tuple[int, int] = (0, 0)
        self.orientation = GameConstants.Orientation.UNKNOWN

        self.vision_data: list[Tile] = []
        self.vison_level: int = 0

        self.nearest_food: Optional[TilePos] = None
        self.visible_resources: list[ResourcePos] = []

    def set_dimension(self, width: int, height: int) -> None:
        """Set internal world dimensions"""
        self.width = width
        self.height = height

    def update_vison(self, vision_str: str) -> None:
        """Update vison data and try to find the nearest food"""
        self.vision_data = MessageParser.parse_vision(vision_str)
        self._analyse_vision()

    def _analyse_vision(self) -> None:
        """Analyse current vision data to find food and resources"""
        food_positions: list[TilePos] = []
        resources_positions: list[ResourcePos] = []

        for i, tile_items in enumerate(self.vision_data):
            tile_pos: RelativePos = self._get_relative_position(i)

            for resource in tile_items:
                if resource == "food":
                    food_positions.append((tile_pos, i))
                # elif resource in GameConstants.RESOURCES:
                # avoid checking for food
                resources_positions.append((resource, tile_pos, i))

        self.nearest_food = self._find_nearest_food(food_positions)
        self.visible_resources = resources_positions


    def _get_relative_position(self, tile_index: int) -> RelativePos:
        """Convert tile index to relative position based on vison level"""

        # Simple level 1 vision:
        if self.vison_level == 1:
            if tile_index == 0:
                return (-1, -1)
            elif tile_index == 1:
                return (-1, 0)
            elif tile_index == 2:
                return (-1, 1)

        return (0, 0)

    def _find_nearest_food(self, food_positions: list[TilePos]) -> Optional[TilePos]:
        """Find the nearest food from current position"""
        if not food_positions:
            return None

        # For now return the first food found
        return food_positions[0]

    def update_player_position(self, x: int, y: int,
                               direction: Optional[GameConstants.Orientation]) \
                               -> None:
        self.player_pos = (x, y)
        self.orientation = direction if direction else self.orientation

    @property
    def get_player_position(self) -> tuple[int, int, GameConstants.Orientation]:
        return (self.player_pos[0], self.player_pos[1], self.orientation)

    @property
    def is_food_visible(self) -> bool:
        return self.nearest_food is not None

    def get_tile_content(self, tile_index: int) -> Tile:
        """Get the content of a specific tile"""
        if 0 <= tile_index < len(self.vision_data):
            return self.vision_data[tile_index]
        raise IndexError("Index out of range")

    def get_resource_positions(self, resource_type: str) -> list[TilePos]:
        """Get a list of all tile positions containing the specified resource type"""
        resource_lst: list[TilePos] = []
        for resource in self.visible_resources:
            if resource[0] == resource_type:
                resource_lst.append((resource[1], resource[2]))
        return resource_lst
