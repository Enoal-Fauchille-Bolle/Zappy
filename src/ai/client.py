##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## client
##

from typing import Optional, Any, cast, TypeAlias
from enum import Enum
import asyncio
import random
import hashlib
import time
from sys import stderr
from collections import deque
from .network import NetworkManager
from .generator import ProcessManager
from .utils import MessageParser, GameConstants, Direction
from .vision import PlayerVision, TilePos
from .protocol import TeamProtocol, MessageType, Role, ElevationCoordinator, MsgData

RoleData: TypeAlias = dict[str, Any]

class ZappyAi:
    "Main AI client class that manage the game logic"

    class ActionState(Enum):
        """"""
        SURVIVAL = 0
        RESOURCE_GATHERING = 1
        REPRODUCTION = 2
        PREPARING_ELEVATION = 3
        ELEVATING = 4
        COORDINATING = 5
        EXPLORATION = 6
        ROLE_EXECUTION = 7
        SYNCHRONIZING = 8
        IDLE = 10

    def __init__(self, hostname: str, port: int, team_name: str) -> None:
        self.hostname = hostname
        self.port = port
        self.team_name = team_name

        self.connexion = NetworkManager(hostname, port)
        self.vision = PlayerVision()
        self.process_manager = ProcessManager()

        self.running: bool = False
        self.map_size: tuple[int, int] = (0, 0)
        self.pending_response: deque[str] = deque(maxlen=10)

        self.level: int = 1
        self.player_id = int(hashlib.md5(f"{time.time()}{random.random()}"
                                         .encode()).hexdigest()[:8], 16)
        self.inventory: dict[str, int] = {
            "food": 10,
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }

        self.protocol = TeamProtocol(team_name)
        self.elevator = ElevationCoordinator()

        self.current_role = Role.NO_ROLE
        self.role_data: RoleData = {}

        self.team_members_data: dict[int, RoleData] = {}
        self.leader_id: Optional[int] = None

        self.team_resources: dict[str, int] = {
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }

        self.state = self.ActionState.IDLE

        self.current_plan: list[str] = []
        self.planned_action: int = 0
        self.look_cooldown: int = 0
        self.inventory_cooldown: int = 0
        self.exploration_cooldown: int = 0
        self.broadcast_cooldown: int = 0

        self.exploration_steps: int = 0
        self.max_steps: int = 5

        self.survival_threshold: int = 5
        self.reproduction_threshold: int = 13
        self.resource_threshold: int = 7

        self.has_layed: bool = False
        self.lay_cooldown: int = 0

        self.elevation_target: Optional[int] = None
        self.meeting_point: Optional[tuple[int, int]] = None
        self.elevation_time: Optional[int] = None

    async def connect_to_server(self) -> None:
        """Connect to the Zappy server"""
        await self.connexion.connect()

        welcome_msg= cast(str, await self.connexion.receive())
        if welcome_msg != "WELCOME":
            raise Exception(f"Expected WELCOME, got: {welcome_msg}")
        if not self.connexion.send(self.team_name):
            raise Exception

        open_slot_remaining_str = cast(str, await self.connexion.receive())
        if open_slot_remaining_str == "ko":
            raise Exception("The team name isn't allowed")
        world_size_str = cast(str, await self.connexion.receive())
        try:
            open_slots = int(open_slot_remaining_str)
            world_size: list[str] = world_size_str.split()
            self.map_size: tuple[int, int] = (int(world_size[0]), int(world_size[1]))
            if open_slots != 0 and self.current_role != Role.FEEDER:
                self.process_manager.spawn_ai_process(
                    self.hostname, self.port, self.team_name
                )
        except Exception as e:
            raise Exception(f"Welcoming was not respected: {e}")
        print(f"Ai {self.player_id} is now connected !")

    async def cleanup(self) -> None:
        """Close Connection and stop the AI(s)"""
        self.running = False
        await self.connexion.disconnect()
        self.process_manager.shutdown_all()

    async def run(self) -> None:
        """Main Ai execution loop"""
        print(f"AI starting in team {self.team_name}")
        try:
            if not self.connexion.is_connected:
                await self.connect_to_server()

            self.running = True
            await self._game_loop()

        except Exception as e:
            print(f"There was an error in main loop: {e}", file=stderr)
        finally:
            await self.cleanup()

    async def _game_loop(self) -> None:
        """AI execution loop"""
        while self.running:
            self._handle_messages()
            self._plan_actions()
            await asyncio.sleep(1/120)

    def _handle_messages(self) -> None:
        """Handle incoming messages"""
        messages = self.connexion.receive_all()
        if messages:
            print(f"Received: {messages}")
        for message in messages:
            self._process_message(message)

    def _plan_actions(self) -> None:
        """Plan and execute actions"""
        self._update_action_state()

        if len(self.pending_response) < 10:
            if "Look" in self.pending_response:
                return
            next_action: Optional[str] = self._plan_next_action()
            if next_action:
                print(f"Next action: {next_action}")
                if self.connexion.send(next_action):
                    self.pending_response.append(next_action)
                else:
                    print("Failed to execute action", file=stderr)

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
            self._update_cds()
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
        (direction, text) = MessageParser.parse_broadcast(message)

        msg = self.protocol.parse_message(text)
        if msg:
            self._handle_message(Direction(direction), msg)
        else:
            print(f"Non team broadcast from {Direction(direction)}, {text}")

    def _handle_message(self, direction: Direction, message: MsgData) -> None:
        """Process a team message based on type"""
        msg_type = MessageType(message["type"])
        sender_id: int = message["sender"]
        data: RoleData = message["data"]

        self.team_members_data[sender_id] = {
            "level": data.get("level", 1),
            "role": Role(data.get("role", Role.NO_ROLE.value))
        }

        if msg_type == MessageType.ROLE_ASSIGNMENT:
            self._handle_role_assignement(direction, sender_id, data)
        elif msg_type == MessageType.RESOURCE_SHARE:
            self._handle_resource_share(sender_id, data)
        elif msg_type == MessageType.ELEVATION_CALL:
            self._handle_elevation_call(sender_id, data)
        elif msg_type == MessageType.LEADER_ELECTION:
            self._handle_leader_election(sender_id)
        elif msg_type == MessageType.SYNC_REQUEST:
            self._handle_sync_request()

    def _handle_role_assignement(self, direction: Direction, send_id: int, data: RoleData) -> None:
        """Handle role assignement messages"""
        if direction != Direction.CENTER:
            return

        new_role = Role(data["new_role"])
        target_id = data.get("target_id", None)

        if target_id and target_id != self.player_id:
            return
        if new_role.value < self.current_role.value and send_id != self.leader_id:
            return

        self.current_role = new_role
        self.role_data = data.get("role_data", {})

        print(f"Ai {self.player_id} assigned role: {new_role.name}")

        if new_role == Role.FEEDER:
            self._execute_feeder_role()

    def _handle_resource_share(self, send_id: int, data: RoleData) -> None:
        """Update team resource knowledge"""
        resources: dict[str, int] = data["resources"]
        for resource, amount in resources.items():
            self.team_resources[resource] = max(self.team_resources[resource], amount)

    def _handle_elevation_call(self, send_id: int, data: RoleData) -> None:
        """Handle elevation coordination calls"""
        level: int = data["targeted_level"]
        needed_players = data["needed_players"]

        if self.level == level - 1 and self.player_id in needed_players:
            self.elevation_target = level
            self.state = self.ActionState.PREPARING_ELEVATION

    def _handle_leader_election(self, send_id: int) -> None:
        """Handle leader election process"""
        self.leader_id = send_id
        print(f"Ai {self.player_id} accepts {send_id} as leader")

    def _handle_sync_request(self) -> None:
        """Respond to synchronization requests"""
        if self.broadcast_cooldown > 0:
            return

        status_data: RoleData = {
            "level": self.level,
            "role": self.current_role.value,
            "resources": {k: v for k, v in self.inventory.items() if k != "food"},
        }

        msg = self.protocol.create_message(
            MessageType.STATUS_UPDATE,
            self.player_id,
            status_data
        )

        self.current_plan.append(f"Broadcast {msg}")
        self.broadcast_cooldown = 10

    def _execute_feeder_role(self) -> None:
        """Feeder role - drop food and prepare to die"""
        self.current_plan.clear()

        food_count = self.inventory["food"]
        for _ in range(food_count):
            self.current_plan.append("Set food")

        self.state = self.ActionState.IDLE

    def _handle_eject(self, message: str) -> None:
        """Update player position after ejection"""

    def _handle_basic(self, message: str, command: str) -> None:
        """Handle if the command succedded or not"""
        if message == "ok":
            if command == "Fork":
                self.has_layed = True
                self.lay_cooldown = 100
                self.process_manager.spawn_ai_process(
                    self.hostname, self.port, self.team_name
                )
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

    def _update_cds(self) -> None:
        """Update all cooldown timers"""
        self.look_cooldown = max(0, self.look_cooldown - 1)
        self.inventory_cooldown = max(0, self.inventory_cooldown - 1)
        self.lay_cooldown = max(0, self.lay_cooldown - 1)
        self.exploration_cooldown = max(0, self.exploration_cooldown - 1)

    def _check_leader_election(self) -> None:
        if not self.leader_id:
            status_data: RoleData = {
                "level": self.level,
                "role": self.current_role.value,
            }

            msg = self.protocol.create_message(
                MessageType.LEADER_ELECTION,
                self.player_id,
                status_data
            )

            self.current_plan.append(f"Broadcast {msg}")
            self.leader_id = self.player_id
            self.current_role = Role.LEADER

    def _coordinate_elevation(self) -> None:
        """Leader coordinate team elevations"""
        if self.current_role != Role.LEADER:
            return

        team_levels: list[int] = [self.level]
        for member_id, info in self.team_members_data.items():
            team_levels.append(info["level"])

        target_level = self.elevator.get_next_elevation_target(
            team_levels
        )
        if not target_level:
            return

        eligible_players = [self.player_id] if self.level == target_level else []
        for member_id, info in self.team_members_data.items():
            if info["level"] == target_level - 1:
                eligible_players.append(member_id)
        elevation_plan = self.elevator.plan_elevation(
            target_level,
            eligible_players,
            self.team_resources
        )

        if elevation_plan:
            elevation_plan["start_time"] = time.time() + 3
            elevation_data: RoleData = {
                "level": self.level,
                "role": self.current_role.value,
                "targeted_level": target_level,
                "start_time": elevation_plan["start_time"],
                "needed_players": elevation_plan["players"],
                "resources_needed": elevation_plan["resources"]
            }

            msg = self.protocol.create_message(
                MessageType.ELEVATION_CALL,
                self.player_id,
                elevation_data
            )

            self.current_plan.append(f"Broadcast {msg}")
            print(f"Leader {self.player_id} calling elevation for level {target_level}")

    def _spawn_feeder_if_needed(self) -> bool:
        """Spawn a feeder AI if food is critically low"""
        if self.inventory["food"] <= 3:
            self.current_plan.insert(0, "Fork")
            feeder_data: RoleData = {
                "level": self.level,
                "role": self.current_role.value,
                "new_role": Role.FEEDER.value,
                "role_data": {"parent_id": self.player_id}
            }

            msg = self.protocol.create_message(
                MessageType.ROLE_ASSIGNMENT,
                self.player_id,
                feeder_data
            )

            self.current_plan.insert(1, f"Broadcast {msg}")
            return True
        return False

    def _update_action_state(self) -> None:
        """Update the current state based on resources"""
        food_count: int = self.inventory["food"]

        # self.state = self.ActionState.SURVIVAL
        if food_count <= self.survival_threshold:
            self.state = self.ActionState.SURVIVAL
            return
        elif food_count >= self.reproduction_threshold and not self.has_layed:
            self.state = self.ActionState.REPRODUCTION
            return
        elif food_count >= self.resource_threshold:
            self.state = self.ActionState.RESOURCE_GATHERING
            return
        else:
            self.state = self.ActionState.SURVIVAL

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


    def _plan_survival(self) -> Optional[str]:
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

    def _plan_resource_gathering(self) -> Optional[str]:
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

    def _plan_exploration(self) -> Optional[str]:
        """Plan exloration actions to discover new areas"""
        if self.exploration_cooldown > 0:
            return None
        self.exploration_cooldown = 1
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

    def get_info(self) -> dict[str, Any]:
        """Get debug information about the AI state"""
        return {
            "level": self.level,
            "has layed": self.has_layed,
            "pending_commands": list(self.pending_response),
            "current_plan": self.current_plan.copy()
        }
