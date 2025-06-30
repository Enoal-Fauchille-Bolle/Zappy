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

MsgData: TypeAlias = dict[str, Any]
MsgHeader: TypeAlias = dict[str, str | int | MsgData]

class MessageType(Enum):
    """Types of messages that can be broadcast"""
    READY_FOR_ELEVATION = "ready_elevation"
    LOCATION_BEACON = "location_beacon"
    ARRIVED_AT_LOCATION = "arrived"
    START_ELEVATION = "start_elevation"
    ASSIGN_FEEDER = "assign_feeder"
    FOOD_DROPPED = "food_dropped"
    ELEVATION_SUCCESS = "elevation_success"
    SEARCHING_RESOURCES = "searching"

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
                      data: MsgData) -> str:
        """Create an encrypted team message"""
        message: MsgHeader = {
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
