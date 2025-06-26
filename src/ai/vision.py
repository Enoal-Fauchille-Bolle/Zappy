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
        self.vision_data: list[Tile] = []

        self.visible_resources: list[ResourcePos] = []
        self.visible_players: list[TilePos] = []
        self.last_vision_update: int = 0

    def update_vison(self, vision_str: Optional[str]) -> None:
        """Update vison data and try to find the nearest food"""
        if vision_str:
            self.vision_data = MessageParser.parse_vision(vision_str)
        self._analyse_vision()
        self.last_vision_update += 1

    def _analyse_vision(self) -> None:
        """Analyse current vision data to find food and resources"""
        players_position: list[TilePos] = []
        resources_positions: list[ResourcePos] = []

        for i, tile_items in enumerate(self.vision_data):
            tile_pos: RelativePos = self._get_relative_position(i)

            for resource in tile_items:
                if resource in GameConstants.RESOURCES: # ignore player or eggs
                    resources_positions.append((resource, tile_pos, i))
                else:
                    players_position.append((tile_pos, i))
        self.visible_resources = resources_positions
        self.visible_players = players_position

    def _get_relative_position(self, tile_index: int) -> RelativePos:
        """Convert tile index to relative position based on vison level"""
        if tile_index < 0 or tile_index >= len(self.vision_data):
            raise IndexError("Tile index out of range")

        if tile_index == 0:
            return (0, 0)  # Player position

        row = 1
        row_width = row * 2 + 1
        remaining_index = tile_index - 1  # Subtract 1 for player tile

        while tile_index > row_width:
            remaining_index -= row_width
            row += 1
            row_width = row * 2 + 1

        x = remaining_index - row
        y = -row
        return (x, y)

    def clear_vison(self) -> None:
        """Clear the current vision_data"""
        self.vision_data.clear()
        self.visible_resources.clear()
        self.visible_players.clear()

    def find_nearest_resource(self, resource_type: str) -> Optional[TilePos]:
        """Find the nearest resource of a specific type"""
        if not self.visible_resources:
            return None

        nearest_resource: Optional[TilePos] = None
        min_distance: float = float('inf')

        for resource in self.visible_resources:
            if resource[0] == resource_type:
                distance = abs(resource[1][0]) + abs(resource[1][1])
                if distance < min_distance:
                    min_distance = distance
                    nearest_resource = (resource[1], resource[2])

        return nearest_resource

    def find_all_resource(self, resource_type: str,
                          max_count: Optional[int] = None) -> list[TilePos]:
        """Find all instance of a resource type, sorted by distance"""
        matching_resources: list[tuple[int, TilePos]] = []

        for resource in self.visible_resources:
            if resource[0] == resource_type:
                distance = abs(resource[1][0]) + abs(resource[1][1])
                matching_resources.append((distance, (resource[1], resource[2])))

        matching_resources.sort(key=lambda x: x[0])
        return [pos[1] for pos in matching_resources[:max_count]]

    # def get_resource_density(self, resource_type: str) -> float:
    #     """Calculate the density of a specific resource in the visible area"""
    #     total_tiles = len(self.vision_data) - 1  # Exclude player tile
    #     resource_count = sum(1 for res in self.visible_resources if res[0] == resource_type)
    #     return resource_count / total_tiles if total_tiles > 0 else 0.0

    # def is_tile_empty(self, tile_index: int) -> bool:
    #     """Check if a specific tile is empty"""
    #     if 0 <= tile_index < len(self.vision_data):
    #         tile_content = self.vision_data[tile_index]
    #         # Tile is empty if it contains no items or only players
    #         return len([item for item in tile_content if item != "player"]) == 0
    #     return True

    def get_safe_exploration_direction(self) -> Optional[RelativePos]:
        """Find a safe direction for exploration (towards areas with fewer players)"""
        directions = [(0, -1), (1, 0), (0, 1), (-1, 0)]  # North, East, South, West
        best_direction = None
        min_player_count = float('inf')

        for direction in directions:
            player_count = 0
            # Check tiles in this direction
            for i, tile_items in enumerate(self.vision_data):
                if i == 0:  # Skip player's current tile
                    continue
                tile_pos = self._get_relative_position(i)

                # Check if tile is in the direction we're considering
                if (direction[0] == 0 and tile_pos[0] == 0 and
                    tile_pos[1] * direction[1] > 0) or \
                   (direction[1] == 0 and tile_pos[1] == 0 and
                    tile_pos[0] * direction[0] > 0):
                    player_count += tile_items.count("player")

            if player_count < min_player_count:
                min_player_count = player_count
                best_direction = direction

        return best_direction

    @property
    def is_food_visible(self) -> bool:
        """Check if any food is visible in the current field of view"""
        return any(resource[0] == "food" for resource in self.visible_resources)

    @property
    def is_player_nearby(self, max_distance: int = 4) -> bool:
        """
        Check if other players are nearby within the specified distance.

        By default, check for a radius of two.
        """
        for position in self.visible_players:
            distance = abs(position[0][0]) + abs(position[0][1])
            if distance <= max_distance:
                return True
        return False

    @property
    def resource_counts(self) -> dict[str, int]:
        """Get a count of all visible resources"""
        resource_counts: dict[str, int] = {}
        for resource in self.visible_resources:
            name = resource[0]
            resource_counts[name] = resource_counts.get(name, 0) + 1
        return resource_counts

    def get_tile_content(self, tile_index: int) -> Tile:
        """Get the content of a specific tile"""
        if 0 <= tile_index < len(self.vision_data):
            return self.vision_data[tile_index]
        raise IndexError("Index out of range")

    def get_vision_summary(self):
        """Get a summary of the current vision"""
        resource_counts = self.resource_counts
        return {
            "total_tiles": len(self.vision_data),
            "resources_visible": resource_counts,
            "players_pos": self.visible_players
        }

if __name__ == "__main__":
    print("zaaaaa")
    vision = PlayerVision()
    vision.visible_resources = [
        ("food", (-1, -1), 1),
        ("thystame", (0, 0), 0),
        ("food", (2, -2), 8)
    ]
    print(f"get_all_resouces: {vision.find_all_resource("food", 1)}")
    print(f"is food: {(resource[0] == "food" for resource in vision.visible_resources)}")
