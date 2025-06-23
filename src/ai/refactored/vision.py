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
        self.vison_level: int = 1

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
        resources_positions: list[ResourcePos] = []

        for i, tile_items in enumerate(self.vision_data):
            tile_pos: RelativePos = self._get_relative_position(i)

            for resource in tile_items:
                resources_positions.append((resource, tile_pos, i))
        self.visible_resources = resources_positions


    def _get_relative_position(self, tile_index: int) -> RelativePos:
        """Convert tile index to relative position based on vison level"""
        if tile_index < 0 or tile_index >= len(self.vision_data):
            raise IndexError("Tile index out of range")
        row = tile_index // (self.vison_level * 2 + 1)
        col = tile_index % (self.vison_level * 2 + 1)
        rel_x = col - self.vison_level
        rel_y = row - self.vison_level
        return (rel_x, rel_y)

    def find_nearest_resource(self, resource_type: str) -> Optional[TilePos]:
        """Find the nearest resource of a specific type"""
        nearest_resource: Optional[TilePos] = None
        min_distance: float = float('inf')
        for resource in self.visible_resources:
            if resource[0] == resource_type:
                distance = (resource[1][0] - self.player_pos[0]) ** 2 + \
                           (resource[1][1] - self.player_pos[1]) ** 2
                if distance < min_distance:
                    min_distance = distance
                    nearest_resource = (resource[1], resource[2])
        return nearest_resource

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
        return self.find_nearest_resource("food") is not None

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
