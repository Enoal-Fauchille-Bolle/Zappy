##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## protocol
##

from typing import Optional, Any, TypeAlias
from enum import Enum
import json
import base64
import time
import hashlib
from .utils import GameConstants

MsgData: TypeAlias = dict[str, Any]
TeamResources: TypeAlias = dict[str, int]

class MessageType(Enum):
    """Types of messages that can be broadcast"""
    ROLE_ASSIGNMENT = "ROLE"
    RESOURCE_SHARE = "RSRC"
    ELEVATION_CALL = "ELEV"
    STATUS_UPDATE = "STAT"
    LEADER_ELECTION = "LEAD"
    DEATH_NOTICE = "DEAD"
    SYNC_REQUEST = "SYNC"

class Role(Enum):
    """AI roles with tier levels (higher value = higher priority)"""
    NO_ROLE = 0
    FEEDER = 1          # Drops food and dies
    GATHERER = 2        # Collects resources
    BREEDER = 3         # Focuses on reproduction
    RESOURCE_SCOUT = 4  # Scouts for resources
    COORDINATOR = 5     # Helps coordinate elevations
    LEADER = 6          # Central coordinator

class TeamProtocol:
    """Handles team communication and message encryption"""

    def __init__(self, team_name: str):
        self.team_name = team_name
        self.secret_key = hashlib.sha256(team_name.encode()).hexdigest()[:16]
        self.message_counter = 0
        self.received_messages: list[tuple[int, str]] = []  # (timestamp, message_id)

    def _generate_message_id(self) -> str:
        """Generate unique message ID"""
        self.message_counter += 1
        return f"{int(time.time())}-{self.message_counter}"

    def _simple_encrypt(self, message: str) -> str:
        """Simple XOR encryption for team messages"""
        encrypted: list[str] = []
        for i, char in enumerate(message):
            key_char = self.secret_key[i % len(self.secret_key)]
            encrypted.append(chr(ord(char) ^ ord(key_char)))
        return base64.b64encode(''.join(encrypted).encode()).decode()

    def _simple_decrypt(self, encrypted: str) -> Optional[str]:
        """Decrypt team messages"""
        try:
            decoded = base64.b64decode(encrypted).decode()
            decrypted: list[str] = []
            for i, char in enumerate(decoded):
                key_char = self.secret_key[i % len(self.secret_key)]
                decrypted.append(chr(ord(char) ^ ord(key_char)))
            return ''.join(decrypted)
        except:
            return None

    def _is_duplicate_message(self, message_id: str) -> bool:
        """Check if we've already processed this message"""
        current_time = int(time.time())
        # Clean old messages (older than 30 seconds)
        self.received_messages = [(t, mid) for t, mid in self.received_messages
                                  if current_time - t < 30]

        # Check if message already received
        for i, (_, mid) in enumerate(self.received_messages):
            if mid == message_id:
                # Update timestamp for existing message
                self.received_messages[i] = (current_time, message_id)
                return True

        # Add to received messages
        self.received_messages.append((current_time, message_id))
        return False

    def create_message(self, msg_type: MessageType, sender_id: int,
                      data: dict[str, Any]) -> str:
        """Create an encrypted team message"""
        message: MsgData = {
            "type": msg_type.value,
            "sender": sender_id,
            "id": self._generate_message_id(),
            "data": data
        }

        json_str = json.dumps(message, separators=(',', ':'))
        encrypted = self._simple_encrypt(json_str)

        # Add team identifier prefix
        return f"T{self.team_name.upper()}:{encrypted}"

    def parse_message(self, broadcast: str) -> Optional[MsgData]:
        """Parse and decrypt a team message"""
        # Check if it's a team message
        if not broadcast.startswith(f"T{self.team_name.upper()}:"):
            return None

        try:
            encrypted = broadcast.split(':', 1)[1]
            decrypted = self._simple_decrypt(encrypted)
            if not decrypted:
                return None

            message = json.loads(decrypted)

            # Check for duplicate
            if self._is_duplicate_message(message["id"]):
                return None

            return message
        except:
            return None

class ElevationCoordinator:
    """Manages elevation requirements and coordination"""

    def __init__(self):
        self.elevation_in_progress: dict[int, dict[str, Any]] = {}
        self.resource_reservations: dict[str, int] = {
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
        self.player_commitments: dict[int, list[int]] = {}  # level -> [player_ids]

    def plan_elevation(self, level: int, available_players: list[int],
                      team_resources: TeamResources) -> Optional[dict[str, Any]]:
        """Plan an elevation if resources and players are available"""

        requirements = GameConstants.ELEVATION_REQUIREMENTS[level - 1]
        needed_players = requirements["players"]

        # Check if we have enough players
        if len(available_players) < needed_players:
            return None

        # Check resources (considering reservations)
        for resource, needed in requirements.items():
            if resource == "players":
                continue
            available = team_resources.get(resource, 0) - self.resource_reservations.get(resource, 0)
            if available < needed:
                return None

        # Create elevation plan
        plan: dict[str, Any] = {
            "level": level,
            "players": available_players,
            "resources": {r: requirements[r] for r in requirements if r != "players"},
            "start_time": None
        }

        # Reserve resources
        for resource, needed in plan["resources"].items():
            self.resource_reservations[resource] += needed

        # Track player commitments
        self.player_commitments[level] = plan["players"]

        return plan

    def cancel_elevation(self, level: int) -> None:
        """Cancel an elevation and free resources"""
        if level in self.elevation_in_progress:
            plan = self.elevation_in_progress[level]
            # Free reserved resources
            for resource, amount in plan["resources"].items():
                self.resource_reservations[resource] -= amount

            del self.elevation_in_progress[level]
            if level in self.player_commitments:
                del self.player_commitments[level]

    def get_next_elevation_target(self, team_levels: list[int]) -> Optional[int]:
        """Determine which level to target next for elevation"""
        level_counts = {i: 0 for i in range(1, 9)}
        for level in team_levels:
            level_counts[level] += 1

        # Priority: Get players to level 8
        for target_level in range(2, 9):
            if level_counts[target_level - 1] >= GameConstants.ELEVATION_REQUIREMENTS[target_level - 1]["players"]:
                return target_level

        return None
