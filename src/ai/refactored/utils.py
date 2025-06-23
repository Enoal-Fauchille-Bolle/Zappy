##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## utils
##

import time
from enum import Enum

class Timer:
    """Simple timer for mesuring execution time"""

    def __init__(self) -> None:
        self.start_time: float = time.time()
        self.lap_time: float = self.start_time

    def lap(self) -> float:
        """Get time since last lap"""
        current_time: float = time.time()
        elasped: float = current_time - self.lap_time
        self.lap_time = current_time
        return elasped

    def total(self) -> float:
        """Get time since timer creation"""
        return time.time() - self.start_time

class GameConstants:
    """Constants from the Zappy specification"""

    AI_HUNGER_CD: int = 126
    DEFAULT_FREQUENCY:int = 100

    RESOURCES: list[str] = [
        "food",
        "linemate",
        "deraumere",
        "sibur",
        "mendiane",
        "phiras",
        "thystame"
    ]

    ELEVATION_REQUIREMENTS: dict[int, dict[str, int]] = {
        1: {"players": 1, "linemate": 1, "deraumere": 0, "sibur" : 0, "mendiane": 0, "phiras": 0, "thystame": 0},
        2: {"players": 2, "linemate": 1, "deraumere": 1, "sibur" : 1, "mendiane": 0, "phiras": 0, "thystame": 0},
        3: {"players": 2, "linemate": 2, "deraumere": 0, "sibur" : 1, "mendiane": 0, "phiras": 2, "thystame": 0},
        4: {"players": 4, "linemate": 1, "deraumere": 1, "sibur" : 2, "mendiane": 0, "phiras": 1, "thystame": 0},
        5: {"players": 4, "linemate": 1, "deraumere": 2, "sibur" : 1, "mendiane": 3, "phiras": 0, "thystame": 0},
        6: {"players": 6, "linemate": 1, "deraumere": 2, "sibur" : 3, "mendiane": 0, "phiras": 1, "thystame": 0},
        7: {"players": 6, "linemate": 2, "deraumere": 2, "sibur" : 2, "mendiane": 2, "phiras": 2, "thystame": 1},
    }

    COMMAND_TIMES: dict[str, int] = {
        "Forward": 7,
        "Right": 7,
        "Left": 7,
        "Look": 7,
        "Inventory": 1,
        "Broadcast": 7,
        "Connect_nbr": 0,
        "Fork": 42,
        "Eject": 7,
        "Take": 7,
        "Set": 7,
        "Incantation": 300
    }

    class Orientation(Enum):
        NORTH = 0
        EAST = 1
        SOUTH = 2
        WEST = 3
        UNKNOWN = 4

class MessageParser:
    """Utility class for parsing server messages"""

    @staticmethod
    def parse_vision(vision_string: str) -> list[list[str]]:
        """Parse vision string into structured data"""
        vision_data: list[list[str]] = []
        tiles: list[str] = vision_string.strip("[]").split(",")
        for tile_content in tiles:
            tile_content = tile_content.strip()
            if tile_content:
                content: list[str] = tile_content.split()
                vision_data.append(content)
            else:
                vision_data.append([])
        return vision_data

    @staticmethod
    def parse_inventory(inventory_string: str) -> dict[str, int]:
        """Parse inventory string into structured data"""
        inventory: dict[str, int] = {}
        items: list[str] = inventory_string.strip("[]").split(",")

        for resource in items:
            name, count = resource.strip().split(" ", 1)
            inventory[name.strip()] = int(count.strip())

        return inventory

    @staticmethod
    def parse_broadcast(message: str) -> tuple[int, str]:
        """Parse broadcast message into direction and message"""
        parts: list[str] = message.split(',')
        direction = int(parts[0].split()[1])
        text = parts[1].strip().strip("\"")
        return (direction, text)

class AIUtils:
    """Utility function for AI behavior"""

    @staticmethod
    def calculate_distance(pos1: tuple[int, int], pos2: tuple[int, int],
                           world_width: int, world_height: int) -> int:
        """
        Calcualte shortest distance between two positions
        (considering wrapping)
        """
        print("calculate_distance not yet implemented")
        return 0

    @staticmethod
    def time_remaining(food_count: int,
                       frequency: int = GameConstants.DEFAULT_FREQUENCY)-> float:
        """Calculate remaining survival time based on food count"""
        return (food_count * GameConstants.AI_HUNGER_CD) / frequency

    @staticmethod
    def is_elevation_possible(level: int, inventory: dict[str, int],
                              nearby_players: int) -> bool:
        """Check if elevation is possible with current resources"""
        if level not in GameConstants.ELEVATION_REQUIREMENTS:
            return False

        requirements = GameConstants.ELEVATION_REQUIREMENTS[level]

        if nearby_players < requirements["players"]:
            return False
        for resource, needed in requirements.items():
            if resource != "players" and inventory[resource] < needed:
                return False
        return True
