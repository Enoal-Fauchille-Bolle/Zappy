##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## client
##

from typing import Optional, cast
import asyncio
from sys import stderr
from collections import deque
from .network import NetworkManager
from .utils import Timer, AIUtils, MessageParser, GameConstants
from .vision import PlayerVision, TilePos

class ZappyAi:
    "Main AI client class that manage the game logic"

    def __init__(self, hostname: str, port: int, team_name: str) -> None:
        self.hostname = hostname
        self.port = port
        self.team_name = team_name

        self.connexion = NetworkManager(hostname, port)
        self.vision = PlayerVision()
        self.clock = Timer()

        self.running: bool = False
        self.connected: bool = False
        self.command_queue: deque[str] = deque(maxlen=10)
        self.pending_response: deque[str] = deque()

        self.player_id: Optional[int] = None
        self.level: int = 1
        self.inventory: dict[str, int] = {
            "food": 10,
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }

        self.current_plan: list[str] = []
        self.planned_action: int = 0
        self.look_cooldown: int = 0
        self.inventory_cooldown: int = 0

    async def connect_to_server(self) -> None:
        """Connect to the Zappy server"""
        await self.connexion.connect()

        welcome_msg= cast(str, await self.connexion.receive())
        if welcome_msg != "WELCOME":
            raise Exception(f"Expected WELCOME, got: {welcome_msg}")
        await self.connexion.send(self.team_name)

        open_slot_remaining_str = cast(str, self.connexion.receive())
        world_size_str = cast(str, self.connexion.receive())
        try:
            open_slot = int(open_slot_remaining_str)
            world_size: list[str] = world_size_str.split()
            self.vision.set_dimension(int(world_size[0]), int(world_size[1]))
        except Exception as e:
            raise Exception(f"Welcoming was not respected: {e}")
        self.connected = True

    async def cleanup(self) -> None:
        """Close Connection and stop the AI"""
        await self.connexion.disconnect()
        self.connected = False
        self.running = False

    async def run(self) -> None:
        """Main Ai execution loop"""
        print(f"AI {self.player_id} starting")
        try:
            if not self.connected:
                await self.connect_to_server()
            self.running = True

            while self.running:
                await self._game_loop()
                await asyncio.sleep(0.01)

        except Exception as e:
            print(f"There was an error in main loop: {e}", file=stderr)
        finally:
            await self.cleanup()

    async def send_command(self, command: str) -> bool:
        """Send a command to the server"""
        if len(self.command_queue) >= 10:
            print("Command queue full, skipping command", file=stderr)
            return False

        await self.connexion.send(command)
        self.command_queue.append(command)
        self.pending_response.append(command)
        return True

    async def _game_loop(self) -> None:
        """Single iteration of the AI execution loop"""
        self.clock.lap()
        await self._handle_incoming_messages()

        if len(self.command_queue) < 10:
            next_action: Optional[str] = self._plan_next_action()
            if next_action:
                await self.send_command(next_action)

    async def _handle_incoming_messages(self) -> None:
        """Check and process all incoming messages from server"""
        messages: list[str] = await self.connexion.receive_all()

        for message in messages:
            self._process_message(message)

    def _process_message(self, message: str) -> None:
        """Process a single message from the server"""

        if message == "dead":
            print("AI died", file=stderr)
            self.running = False
        elif message.startswith("message"):
            self._handle_broadcast(message)
        elif message.startswith("eject"):
            self._handle_eject(message)
        elif self.pending_response:
            command: str = self.pending_response.popleft()
            if command == "Look":
                self.vision.update_vison(message)
            elif command == "Inventory":
                self.inventory = MessageParser.parse_inventory(message)
            else:
                self._handle_basic(message, command)
        else:
            raise Exception(f"Unexpected server message: {message}")

    def _handle_broadcast(self, message: str) -> None:
        """Read and decrypt team broadcasts, otherwise, discard the information"""
        encrypted: tuple[int, str] = MessageParser.parse_broadcast(message)
        print(f"Received {encrypted[1]} from direction {encrypted[0]}")


    def _handle_eject(self, message: str) -> None:
        """Update player position after ejection"""

    def _handle_basic(self, message: str, command: str) -> None:
        """Handle if the command succedded or not"""
        if message == "ok":
            print(f"Command '{command}' executed successfully")
        elif message == "ko":
            print(f"Command '{command}' failed")

    def _plan_next_action(self) -> Optional[str]:
        """Plan the next action to do"""
        self.look_cooldown -= 1
        self.inventory_cooldown -= 1
        if self.current_plan:
            self.planned_action += 1
            return self.current_plan.pop(0)

        if self.inventory["food"] <= 3:
            self._plan_food_collection()
            return

        if self.vision.is_food_visible:

            return

        if self.inventory_cooldown <= 0:
            self.current_plan.append("Inventory")
            self.inventory_cooldown = 5
        if self.look_cooldown <= 0:
            self.look_cooldown = 2 * self.level + 2
            if self.planned_action < self.look_cooldown // 2:
                self.current_plan.append("Right")
            self.current_plan.append("Look")
            self.planned_action = 0

    def _plan_food_collection(self) -> None:
        """"""
        if not self.vision.is_food_visible:
            if self.look_cooldown >= 7:
                self.current_plan.append("Right")
            self.current_plan.append("Look")
            self.look_cooldown = 10
        self._plan_path_to_tile(cast(TilePos,
            self.vision.find_nearest_resource("food")))

    def _plan_path_to_tile(self, tile: TilePos):
        """"""
        self.current_plan.append("Forward")
