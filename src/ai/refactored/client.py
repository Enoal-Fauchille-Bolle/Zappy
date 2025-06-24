##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## client
##

from typing import Optional, cast
from enum import Enum
import asyncio
from sys import stderr
from collections import deque
from .network import NetworkManager
from .utils import Timer, AIUtils, MessageParser, GameConstants
from .vision import PlayerVision, TilePos
import random

class ZappyAi:
    "Main AI client class that manage the game logic"

    class ActionState(Enum):
        """"""
        SURVIVAL = 0
        EXPLORATION = 1
        RESOURCE_GATHERING = 2
        REPRODUCTION = 3
        IDLE = 4

    def __init__(self, hostname: str, port: int, team_name: str) -> None:
        self.hostname = hostname
        self.port = port
        self.team_name = team_name

        self.connexion = NetworkManager(hostname, port)
        self.vision = PlayerVision()
        self.clock = Timer()

        self.running: bool = False
        self.connected: bool = False
        self.pending_response: deque[str] = deque(maxlen=10)

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

        self.exploration_steps: int = 0
        self.max_steps: int = 5

        self.survival_threshold: int = 3
        self.reproduction_threshold: int = 13
        self.resource_threshold: int = 7

        self.has_layed: bool = False
        self.lay_cooldown: int = 0

        self.state = self.ActionState.IDLE

    async def connect_to_server(self) -> None:
        """Connect to the Zappy server"""
        await self.connexion.connect()

        welcome_msg= cast(str, await self.connexion.receive())
        if welcome_msg != "WELCOME":
            raise Exception(f"Expected WELCOME, got: {welcome_msg}")
        await self.connexion.send(self.team_name)

        open_slot_remaining_str = cast(str, await self.connexion.receive())
        if open_slot_remaining_str == "ko":
            raise Exception("The team name isn't allowed")
        world_size_str = cast(str, await self.connexion.receive())
        try:
            open_slots = int(open_slot_remaining_str)
            world_size: list[str] = world_size_str.split()
            self.map_size: tuple[int, int] = (int(world_size[0]), int(world_size[1]))
            if open_slots != 0:
                asyncio.create_task(self._spawn_new_ai())
        except Exception as e:
            raise Exception(f"Welcoming was not respected: {e}")
        print("Ai is now connected !")
        self.connected = True

    async def _spawn_new_ai(self) -> None:
        """Spawn a new AI instance"""
        try:
            new_ai = ZappyAi(self.hostname, self.port, self.team_name)
            await new_ai.run()
        except Exception as e:
            print(f"Error: {e}", file=stderr)

    async def cleanup(self) -> None:
        """Close Connection and stop the AI"""
        await self.connexion.disconnect()
        self.connected = False
        self.running = False

    async def run(self) -> None:
        """Main Ai execution loop"""
        print(f"AI {self.player_id} starting in team {self.team_name}")
        try:
            if not self.connected:
                await self.connect_to_server()
            self.running = True

            while self.running:
                await self._game_loop()
                await asyncio.sleep(1/120)

        except Exception as e:
            print(f"There was an error in main loop: {e}", file=stderr)
        finally:
            await self.cleanup()

    async def send_command(self, command: str) -> bool:
        """Send a command to the server"""
        if len(self.pending_response) >= 10:
            print("Command queue full, skipping command", file=stderr)
            return False

        await self.connexion.send(command)
        self.pending_response.append(command)
        return True

    async def _game_loop(self) -> None:
        """Single iteration of the AI execution loop"""
        await self._handle_incoming_messages()
        # if self.clock.lap(False) > 1:
        #     self.clock.lap()
        #     self._update_cds()
        self._update_action_state()

        if len(self.pending_response) < 10:
            if "Look" in self.pending_response:
                return
            next_action: Optional[str] = self._plan_next_action()
            if next_action:
                print(f"Next action: {next_action}")
                await self.send_command(next_action)

    def _update_cds(self) -> None:
        """Update all cooldown timers"""
        self.look_cooldown = max(0, self.look_cooldown - 1)
        self.inventory_cooldown = max(0, self.inventory_cooldown - 1)
        self.lay_cooldown = max(0, self.lay_cooldown - 1)

    def _update_action_state(self) -> None:
        """Update the current state based on resources"""
        food_count: int = self.inventory["food"]

        # self.state = self.ActionState.SURVIVAL
        if food_count <= self.survival_threshold:
            self.state = self.ActionState.SURVIVAL
            return
        elif food_count >= self.reproduction_threshold:
            self.state = self.ActionState.REPRODUCTION
            return
        elif food_count >= self.resource_threshold:
            self.state = self.ActionState.RESOURCE_GATHERING
            return
        elif random.random() <= 0.3:
            self.state = self.ActionState.EXPLORATION
        else:
            self.state = self.ActionState.SURVIVAL

    async def _handle_incoming_messages(self) -> None:
        """Check and process all incoming messages from server"""
        messages: list[str] = await self.connexion.receive_all()
        if messages:
            print(f"Received: {messages}")

        for message in messages:
            self._process_message(message)
            self._update_cds()

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
                # self.look_cooldown = 0
            elif command == "Inventory":
                self.inventory = MessageParser.parse_inventory(message)
                # self.inventory_cooldown = 0
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
            if command == "Fork":
                self.has_layed = True
                self.lay_cooldown = 100
                asyncio.create_task(self._spawn_new_ai())
            elif command in ["Forward", "Right", "Left"]:
                self.exploration_steps += 1
            print(f"Command '{command}' executed successfully")
        elif message == "ko":
            if command.startswith("Take"):
                pass
            elif command.startswith("Set"):
                self.current_plan.clear()
                self.current_plan.insert(0, "Inventory")
                self.inventory_cooldown = 8
            print(f"Command '{command}' failed")

    def _plan_next_action(self) -> Optional[str]:
        """Plan the next action to do"""
        if self.current_plan:
            return self.current_plan.pop(0)

        if self.inventory_cooldown <= 0:
            self.inventory_cooldown = 8
            return "Inventory"

        if self.look_cooldown <= 0 or not self.vision.vision_data:
            self.look_cooldown = 2 * self.level + 2
            return "Look"

        if self.state == self.ActionState.SURVIVAL:
            return self._plan_survival()
        elif self.state == self.ActionState.REPRODUCTION:
            return self._plan_reproduction()
        elif self.state == self.ActionState.RESOURCE_GATHERING:
            return self._plan_resource_gathering()
        elif self.state == self.ActionState.EXPLORATION:
            return self._plan_exploration()
        else: #state == IDLE
            return None


    def _plan_survival(self) -> str:
        """Plan actions focused on survival (finding food)"""
        food_pos = self.vision.find_nearest_resource("food")

        if food_pos:
            path = self._plan_path_to_tile(food_pos)
            if path:
                self.current_plan.extend(path)
                self.current_plan.append("Take food")
                print(f"Taking food by going to {self.current_plan}")
                self.vision.clear_vison()
                return self.current_plan.pop(0)
            else:
                self.vision.vision_data.pop(food_pos[1])
                self.vision.update_vison(None)
                return "Take food"
        else:
            self.vision.clear_vison()
            return self._plan_exploration()

    def _plan_reproduction(self) -> str:
        """Plan actions needed for reproduction"""
        return "Fork"

    def _plan_resource_gathering(self) -> str:
        """Plan actions to gather resources needed for elevation"""
        requirement = GameConstants.ELEVATION_REQUIREMENTS[self.level]

        for resource_name in [r for r in GameConstants.RESOURCES if r != "food"]:
            needed = requirement[resource_name]
            current = self.inventory[resource_name]

            if current < needed:
                resource_pos = self.vision.find_nearest_resource(resource_name)
                if resource_pos:
                    path = self._plan_path_to_tile(resource_pos)
                    if path:
                        self.current_plan.extend(path)
                        self.current_plan.append(f"Take {resource_name}")
                        self.vision.clear_vison()
                        return self.current_plan.pop(0)
                    else:
                        self.vision.vision_data.pop(resource_pos[1])
                        self.vision.update_vison(None)
                        return f"Take {resource_name}"
        return self._plan_exploration()

    def _plan_exploration(self) -> str:
        """Plan exloration actions to discover new areas"""
        if self.exploration_steps >= self.max_steps:
            self.exploration_steps = 0
            self.max_steps = random.randint(2, 5)
            return "Left"
        else:
            return "Forward"

    def _plan_path_to_tile(self, tile: TilePos,
                           reset_orientation: bool = False) -> list[str]:
        """Plan a path to reach a specific tile"""
        path: list[str] = []
        rel_x, rel_y = tile[0]

        path.extend(["Forward"] * abs(rel_y))
        if rel_x > 0:
            path.append("Right")
            path.extend(["Forward"] * rel_x)
            if reset_orientation:
                path.append("Left")
        elif rel_x < 0:
            path.append("Left")
            path.extend(["Forward"] * abs(rel_x))
            if reset_orientation:
                path.append("Right")

        return path

    @property
    def survival_status(self) -> str:
        """Get current survival status"""
        food_time = AIUtils.time_remaining(self.inventory["food"])
        return f"food: {self.inventory["food"]}, Time left: {food_time}s, " \
            f"State: {self.state.name}"

    def get_info(self):
        """Get debug information about the AI state"""
        return {
            "level": self.level,
            "has layed": self.has_layed,
            "state": self.survival_status,
            "pending_commands": self.pending_response,
            "current_plan": self.current_plan
        }
