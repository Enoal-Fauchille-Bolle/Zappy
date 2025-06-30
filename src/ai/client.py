##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## client
##

from typing import Optional, Any, cast
from enum import Enum
import asyncio
import random
from sys import stderr
from collections import deque
from .network import NetworkManager
from .generator import ProcessManager
from .utils import AIUtils, MessageParser, GameConstants
from .vision import PlayerVision, TilePos
from .protocol import TeamProtocol, MessageType

class ZappyAi:
    "Main AI client class that manage the game logic"

    class ActionState(Enum):
        """"""
        SURVIVAL = 0
        EXPLORATION = 1
        RESOURCE_GATHERING = 2
        REPRODUCTION = 3
        IDLE = 4
        READY_FOR_ELEVATION = 5
        MOVING_TO_PARTNER = 6
        WAITING_FOR_PARTNER = 7
        ELEVATING = 8
        FEEDER = 9

    def __init__(self, hostname: str, port: int, team_name: str) -> None:
        self.hostname = hostname
        self.port = port
        self.team_name = team_name

        self.connexion = NetworkManager(hostname, port)
        self.vision = PlayerVision()
        self.process_manager = ProcessManager()
        self.protocol = TeamProtocol(team_name)

        self.running: bool = False
        self.map_size: tuple[int, int] = (0, 0)
        self.pending_response: deque[str] = deque(maxlen=10)

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
        self.exploration_cooldown: int = 0
        self.broadcast_cooldown: int = 0

        self.exploration_steps: int = 0
        self.max_steps: int = 5

        self.survival_threshold: int = 6  # Increased for elevation safety
        self.reproduction_threshold: int = 13
        self.resource_threshold: int = 10

        self.has_layed: bool = False
        self.lay_cooldown: int = 0

        self.state = self.ActionState.IDLE

        # Coordination variables
        self.player_id: int = random.randint(1000, 9999)
        self.partner_id: Optional[int] = None
        self.partner_direction: Optional[int] = None
        self.beacon_counter: int = 0
        self.elevation_ready: bool = False
        self.partner_ready: bool = False
        self.is_feeder: bool = False
        self.waiting_for_partner: bool = False
        self.elevation_in_progress: bool = False
        self.last_broadcast_direction: Optional[int] = None
        self.steps_since_broadcast: int = 0

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
            if open_slots != 0:
                self.process_manager.spawn_ai_process(
                    self.hostname, self.port, self.team_name
                )
        except Exception as e:
            raise Exception(f"Welcoming was not respected: {e}")
        print(f"AI {self.player_id} is now connected!")

    async def cleanup(self) -> None:
        """Close Connection and stop the AI(s)"""
        self.running = False
        await self.connexion.disconnect()
        self.process_manager.shutdown_all()

    async def run(self) -> None:
        """Main Ai execution loop"""
        print(f"AI {self.player_id} starting in team {self.team_name}")
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
            print(f"AI {self.player_id} Received: {messages}")
        for message in messages:
            self._process_message(message)

    def _plan_actions(self) -> None:
        """Plan and execute actions"""
        self._update_action_state()

        if len(self.pending_response) < 10:
            # Don't send new commands if waiting for Look response
            if "Look" in self.pending_response:
                return
            next_action: Optional[str] = self._plan_next_action()
            if next_action:
                print(f"AI {self.player_id} Next action: {next_action}")
                if self.connexion.send(next_action):
                    self.pending_response.append(next_action)
                else:
                    print("Failed to execute action", file=stderr)

    def _process_message(self, message: str) -> None:
        """Process a single message from the server"""
        if message == "dead":
            print(f"AI {self.player_id} died", file=stderr)
            self.running = False
        elif message.startswith("message"):
            self._handle_broadcast(message)
        elif message.startswith("eject"):
            self._handle_eject(message)
        elif message.startswith("Elevation underway"):
            self.elevation_in_progress = True
            print(f"AI {self.player_id}: Elevation started!")
        elif message.startswith("Current level:"):
            level_str = message.split(":")[1].strip()
            self.level = int(level_str)
            self.elevation_in_progress = False
            self.elevation_ready = False
            self.partner_ready = False
            print(f"AI {self.player_id}: Elevation complete! Now level {self.level}")
            self._broadcast_message(MessageType.ELEVATION_SUCCESS, {"level": self.level})
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
            print(f"Unexpected server message: {message}")

    def _handle_broadcast(self, message: str) -> None:
        """Read and decrypt team broadcasts"""
        direction, text = MessageParser.parse_broadcast(message)

        # Try to parse as team message
        team_msg = self.protocol.parse_message(text)
        if team_msg:
            sender_id = team_msg["sender"]
            msg_type = team_msg["type"]
            # data = team_msg["data"]

            print(f"AI {self.player_id}: Received {msg_type} from {sender_id} at direction {direction}")

            # Handle feeder assignment
            if msg_type == MessageType.ASSIGN_FEEDER.value and direction == 0:
                if sender_id != self.player_id:
                    self.is_feeder = True
                    self.state = self.ActionState.FEEDER
                    print(f"AI {self.player_id}: Assigned as feeder")
                    return

            # Ignore own messages
            if sender_id == self.player_id:
                return

            self.partner_id = sender_id

            if msg_type == MessageType.READY_FOR_ELEVATION.value:
                if self._check_elevation_requirements():
                    self.state = self.ActionState.MOVING_TO_PARTNER
                    self.last_broadcast_direction = direction
                    self.steps_since_broadcast = 0
                    print(f"AI {self.player_id}: Partner ready, moving to them")

            elif msg_type == MessageType.LOCATION_BEACON.value:
                if self.state == self.ActionState.MOVING_TO_PARTNER:
                    # Update direction to partner
                    self.last_broadcast_direction = direction
                    self.steps_since_broadcast = 0
                    if direction == 0:
                        # We're on the same tile
                        self._broadcast_message(MessageType.ARRIVED_AT_LOCATION, {})
                        self.state = self.ActionState.WAITING_FOR_PARTNER
                        print(f"AI {self.player_id}: Arrived at partner location!")

            elif msg_type == MessageType.ARRIVED_AT_LOCATION.value:
                if self.state == self.ActionState.WAITING_FOR_PARTNER:
                    self.partner_ready = True
                    print(f"AI {self.player_id}: Partner arrived!")

            elif msg_type == MessageType.START_ELEVATION.value:
                if not self.elevation_in_progress:
                    self.current_plan = ["Incantation"]

            elif msg_type == MessageType.FOOD_DROPPED.value:
                if direction == 0:  # Same tile
                    self.current_plan.append("Look")

    def _handle_eject(self, message: str) -> None:
        """Update player position after ejection"""
        # Reset meeting if ejected during coordination
        if self.state in [self.ActionState.WAITING_FOR_PARTNER, self.ActionState.MOVING_TO_PARTNER]:
            self.state = self.ActionState.RESOURCE_GATHERING
            self.partner_ready = False
            self.last_broadcast_direction = None

    def _handle_basic(self, message: str, command: str) -> None:
        """Handle if the command succeeded or not"""
        if message == "ok":
            if command == "Fork":
                self.has_layed = True
                self.lay_cooldown = 100
                # If we need food, assign feeder role to child
                if self.inventory["food"] < 15:
                    self._broadcast_message(MessageType.ASSIGN_FEEDER, {})
                    print(f"AI {self.player_id}: Spawning feeder")
                else:
                    self.process_manager.spawn_ai_process(
                        self.hostname, self.port, self.team_name
                    )
            elif command == "Incantation":
                # Should get elevation message instead
                pass
            elif command in ["Forward", "Right", "Left"]:
                self.exploration_steps += 1
                self.steps_since_broadcast += 1
            elif command.startswith("Set food") and self.is_feeder:
                # Drop all food if feeder
                if self.inventory["food"] > 1:
                    self.current_plan.append("Set food")
                else:
                    self._broadcast_message(MessageType.FOOD_DROPPED, {})
                    print(f"AI {self.player_id}: Feeder dropped all food, dying...")
                    self.running = False
            print(f"Command '{command}' executed successfully")
        elif message == "ko":
            if command == "Incantation":
                print(f"AI {self.player_id}: Elevation failed! Checking requirements...")
                self.elevation_in_progress = False
                self.state = self.ActionState.RESOURCE_GATHERING
                self.elevation_ready = False
                self.partner_ready = False
            elif command.startswith("Take"):
                # Resource not there anymore
                self.current_plan.clear()
                self.vision.clear_vison()
            print(f"Command '{command}' failed")

    def _update_cds(self) -> None:
        """Update all cooldown timers"""
        self.look_cooldown = max(0, self.look_cooldown - 1)
        self.inventory_cooldown = max(0, self.inventory_cooldown - 1)
        self.lay_cooldown = max(0, self.lay_cooldown - 1)
        self.exploration_cooldown = max(0, self.exploration_cooldown - 1)
        self.broadcast_cooldown = max(0, self.broadcast_cooldown - 1)

    def _update_action_state(self) -> None:
        """Update the current state based on resources"""
        if self.is_feeder:
            return

        food_count: int = self.inventory["food"]

        # Don't change state if we're coordinating or elevating
        if self.state in [self.ActionState.WAITING_FOR_PARTNER,
                         self.ActionState.MOVING_TO_PARTNER,
                         self.ActionState.ELEVATING]:
            return

        if self.elevation_in_progress:
            self.state = self.ActionState.ELEVATING
            return

        # Check if ready for elevation
        if self._check_elevation_requirements() and food_count > 10:
            if not self.elevation_ready:
                self.elevation_ready = True
                self.state = self.ActionState.READY_FOR_ELEVATION
                print(f"AI {self.player_id}: Ready for elevation to level {self.level + 1}")
            return

        if food_count <= self.survival_threshold:
            self.state = self.ActionState.SURVIVAL
        elif food_count >= self.reproduction_threshold and not self.has_layed:
            self.state = self.ActionState.REPRODUCTION
        else:
            self.state = self.ActionState.RESOURCE_GATHERING

    def _check_elevation_requirements(self) -> bool:
        """Check if we have enough resources for next elevation"""
        if self.level >= 8:
            return False

        requirements = GameConstants.ELEVATION_REQUIREMENTS[self.level]
        for resource, needed in requirements.items():
            if resource != "players" and self.inventory.get(resource, 0) < needed:
                return False
        return True

    def _plan_next_action(self) -> Optional[str]:
        """Plan the next action to do"""
        if self.current_plan:
            return self.current_plan.pop(0)

        # States that should NOT move or explore
        if self.state == self.ActionState.WAITING_FOR_PARTNER:
            # Only update inventory, look and broadcast
            if self.inventory_cooldown <= 0:
                self.inventory_cooldown = 8
                return "Inventory"

            if self.look_cooldown <= 0:
                self.look_cooldown = 5  # Less frequent when waiting
                return "Look"

            # Broadcast continuously while waiting
            if self.broadcast_cooldown <= 0:
                self._broadcast_message(MessageType.LOCATION_BEACON, {
                    "beacon": self.beacon_counter
                })
                self.beacon_counter += 1
                self.broadcast_cooldown = 2  # Spam broadcasts

                # Check if partner is on our tile
                if any(pos[0] == (0, 0) for pos in self.vision.visible_players):
                    self.partner_ready = True
                    print(f"AI {self.player_id}: Partner detected on same tile!")
                    self._broadcast_message(MessageType.START_ELEVATION, {})
                    return "Incantation"

            return None  # Do nothing else when waiting

        # Regular actions for other states
        if self.inventory_cooldown <= 0:
            self.inventory_cooldown = 8
            return "Inventory"

        if self.look_cooldown <= 0 or not self.vision.vision_data:
            self.look_cooldown = 2 * self.level + 2
            return "Look"

        # Broadcast handling for ready state
        if self.broadcast_cooldown <= 0:
            if self.state == self.ActionState.READY_FOR_ELEVATION:
                self._broadcast_message(MessageType.READY_FOR_ELEVATION, {
                    "level": self.level,
                    "resources": "complete"
                })
                self.broadcast_cooldown = 5
                self.state = self.ActionState.WAITING_FOR_PARTNER
                return None  # Stop all movement immediately

        # State-based actions
        if self.state == self.ActionState.SURVIVAL:
            return self._plan_survival()
        elif self.state == self.ActionState.REPRODUCTION:
            return self._plan_reproduction()
        elif self.state == self.ActionState.RESOURCE_GATHERING:
            return self._plan_resource_gathering()
        elif self.state == self.ActionState.EXPLORATION:
            return self._plan_exploration()
        elif self.state == self.ActionState.MOVING_TO_PARTNER:
            return self._plan_move_to_partner()
        elif self.state == self.ActionState.FEEDER:
            return self._plan_feeder()
        elif self.state == self.ActionState.ELEVATING:
            return None  # Wait for elevation to complete
        else:
            return None

    def _plan_survival(self) -> Optional[str]:
        """Plan actions focused on survival (finding food)"""
        # Check if we need a feeder
        if self.inventory["food"] <= 3 and not self.has_layed and self.lay_cooldown <= 0:
            return "Fork"

        food_pos = self.vision.find_nearest_resource("food")
        if food_pos:
            path = self._plan_path_to_tile(food_pos)
            if path:
                self.current_plan.extend(path)
                self.current_plan.append("Take food")
                print(f"Taking food by going to {self.current_plan}")
                self.vision.clear_vison()  # Clear vision after planning movement
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
            needed = requirement.get(resource_name, 0)
            current = self.inventory.get(resource_name, 0)

            if current < needed:
                resource_pos = self.vision.find_nearest_resource(resource_name)
                if resource_pos:
                    path = self._plan_path_to_tile(resource_pos)
                    if path:
                        self.current_plan.extend(path)
                        self.current_plan.append(f"Take {resource_name}")
                        self.vision.clear_vison()  # Clear vision after planning movement
                        return self.current_plan.pop(0)
                    else:
                        self.vision.vision_data.pop(resource_pos[1])
                        self.vision.update_vison(None)
                        return f"Take {resource_name}"

        # Also gather food while looking for resources
        if self.inventory["food"] < 15:
            food_pos = self.vision.find_nearest_resource("food")
            if food_pos:
                path = self._plan_path_to_tile(food_pos)
                if path:
                    self.current_plan.extend(path)
                    self.current_plan.append("Take food")
                    self.vision.clear_vison()
                    return self.current_plan.pop(0)
                else:
                    return "Take food"

        return self._plan_exploration()

    def _plan_exploration(self) -> Optional[str]:
        """Plan exploration actions to discover new areas"""
        if self.exploration_cooldown > 0:
            return None
        self.exploration_cooldown = 1
        if self.exploration_steps >= self.max_steps:
            self.exploration_steps = 0
            self.max_steps = random.randint(2, 5)
            return "Left"
        else:
            return "Forward"

    def _plan_move_to_partner(self) -> Optional[str]:
        """Move towards partner based on broadcast direction"""
        # Wait if we haven't received a broadcast recently
        if self.last_broadcast_direction is None or self.steps_since_broadcast > 3:
            return None  # Wait for new broadcast

        if self.last_broadcast_direction == 0:
            # We're on the same tile
            self._broadcast_message(MessageType.ARRIVED_AT_LOCATION, {})
            self.state = self.ActionState.WAITING_FOR_PARTNER
            print(f"AI {self.player_id}: Arrived at partner location!")
            return None

        # Map broadcast direction to ONE movement step
        direction_map = {
            1: "Forward",
            2: ["Forward", "Left"],  # Corner - take two steps
            3: "Left",
            4: ["Left", "Left"],  # Behind - turn around
            5: ["Right", "Right"],  # Behind - turn around
            6: ["Right", "Forward"],  # Corner - take two steps
            7: "Right",
            8: ["Forward", "Right"]  # Corner - take two steps
        }

        if self.last_broadcast_direction in direction_map:
            moves = direction_map[self.last_broadcast_direction]
            if isinstance(moves, list):
                # For corners and behind, plan multiple moves
                self.current_plan = moves[1:]
                return moves[0]
            else:
                return moves

        return "Forward"

    def _plan_feeder(self) -> Optional[str]:
        """Plan actions for feeder role"""
        if self.inventory["food"] > 0:
            return "Set food"
        return None

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

    def _broadcast_message(self, msg_type: MessageType, data: dict[str, Any]) -> None:
        """Send a broadcast message to the team"""
        message = self.protocol.create_message(msg_type, self.player_id, data)
        if self.connexion.send(f"Broadcast {message}"):
            self.pending_response.append(f"Broadcast {message}")
            print(f"AI {self.player_id}: Broadcasting {msg_type.value}")

    @property
    def survival_status(self) -> str:
        """Get current survival status"""
        food_time = AIUtils.time_remaining(self.inventory["food"])
        return f"food: {self.inventory["food"]}, Time left: {food_time:.1f}s, " \
            f"State: {self.state.name}, Level: {self.level}"

    def get_info(self) -> dict[str, Any]:
        """Get debug information about the AI state"""
        return {
            "player_id": self.player_id,
            "level": self.level,
            "has layed": self.has_layed,
            "state": self.survival_status,
            "pending_commands": list(self.pending_response),
            "current_plan": self.current_plan.copy(),
            "partner_id": self.partner_id,
            "elevation_ready": self.elevation_ready,
            "is_feeder": self.is_feeder,
            "last_broadcast_direction": self.last_broadcast_direction,
            "visible_players": self.vision.visible_players
        }
