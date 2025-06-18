##
## EPITECH PROJECT, 2025
## zappy
## File description:
## AI loop for Zappy
##

import random
from commands import Commands
import threading
import time
from utils import ContinuousMonitor
import sys

class Loop:
    def __init__(self, connexion):
        self.connexion = connexion
        self.running = True
        self.inventory = {"food": 10, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0}
        self.look = []
        self.orientation = 0
        self.map_size = None
        self.commands = Commands(connexion)
        self.connexion.ai_instance = self # Store the AI instance in the connexion object
        self.level = 1
        self.time_look = 9
        self.tick = []
        self.lock = threading.Lock()
        self.inventory_lock = threading.Lock()
        self.tick_counter = 0
        self.tick_duration = 0
        self.current_path = []
        self.elevation_requirements = [
            { "players": 1, "linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
            { "players": 2, "linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0},
            { "players": 2, "linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0},
            { "players": 4, "linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0},
            { "players": 4, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0},
            { "players": 6, "linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0},
            { "players": 6, "linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1}]

    def run(self, map_size=None):
        """Main AI execution loop"""
        print("AI starting")
        self.running = True
        self.is_dead = False
        self.map_size = map_size
        self.monitor = ContinuousMonitor(self)
        self.monitor.start()
        try:
            while self.running and not self.is_dead:
                if hasattr(self, 'is_dead') and self.is_dead:
                    print("Death detected in main loop - exiting")
                    break
                try:
                    self.make_decision()
                    # self.commands.Broadcast("\"AI is running\"")
                except Exception as e:
                    if "Not connected" in str(e):
                        print("Connection lost - stopping AI")
                        break
                    print(f"Decision error: {e}")
        except Exception as e:
            print(f"Error in main loop: {e}")
        finally:
            print("AI shutting down - cleanup")
            if hasattr(self, 'monitor'):
                self.monitor.stop()
                try:
                    # Now this will work!
                    self.monitor.join(timeout=1)
                except Exception as e:
                    print(f"Error joining monitor: {e}")

    def get_tick(self):
        if not self.tick:
            self.time_tick = 0
            # print("No ticks recorded yet.")
            return
        total_tick = sum(self.tick)
        count = len(self.tick)
        for i, tick_time in enumerate(self.tick):
            # print(f"Tick {i}: {tick_time}")
            total_tick += self.tick[i]
        self.time_tick = total_tick / count
        # print(f"Average tick time: {self.time_tick}")

    def make_decision(self):
        """Main decision-making function that orchestrates AI behavior"""
        self.connexion.check_connection()
        if not self.connexion.connected:
            print("Connection lost - skipping decision cycle")
            return
        if self.inventory["food"] <= 3:
            while self.inventory["food"] < 10:
                print("Low food level, planning to find food", flush=True)
                print(f"Current path: {self.current_path}", flush=True)
                if self.current_path:
                    success = self.execute_next_command()
                    if not success:
                        self.current_path = []
                    return
                self.plan_next_action("food")
        else:
            # self.time_look += 1
            # if self.time_look == 10:
            self.check_and_update_look()

    def check_and_update_look(self):
        """Check if it's time to perform a look action"""
        self.commands.Look()
        self.time_look = 0
        print(f"Look content: {self.look}", flush=True)

    def execute_next_command(self):
        """Execute the next command in the current path"""
        if not self.current_path:
            return True
        next_command = self.current_path.pop(0)
        print(f"Executing command: {next_command}", flush=True)
        if next_command.startswith("Take"):
            return self.handle_take_command(next_command)
        else:
            return self.handle_movement_command(next_command)

    def handle_take_command(self, command):
        """Handle taking an item"""
        item = command.split(" ")[1]
        self_tile_idx = -1
        for i, tile in enumerate(self.look):
            if "self" in tile:
                self_tile_idx = i
                break
        if self_tile_idx == -1:
            # print("Error: Can't find self position in look data!")
            return False
        if item in self.look[self_tile_idx]:
            success = self.commands.Take(item)
            # print(f"Take {item} result: {success}")
            return success
        else:
            # print(f"Error: Tried to take {item} but not on the same tile (self at {self_tile_idx})!")
            self.commands.Look()
            return False

    def handle_movement_command(self, command):
        """Handle movement commands"""
        success = False
        if command == "Forward":
            success = self.commands.Forward()
            if success:
                self.commands.Look()
            # print(f"Forward result: {success}")
        elif command == "Right":
            success = self.commands.Right()
            if success:
                self.orientation = (self.orientation + 1) % 4
            # print(f"Right result: {success}, new orientation: {self.orientation}")
        elif command == "Left":
            success = self.commands.Left()
            if success:
                self.orientation = (self.orientation - 1) % 4
            # print(f"Left result: {success}, new orientation: {self.orientation}")
        return success

    def plan_next_action(self, resource_name):
        """Plan the next action when there's no current path"""
        path = self.find_resource_path(resource_name)
        if path:
            print(f"Path to food: {path}", flush=True)
            self.current_path = path
        else:
            self.commands.Right()
            self.orientation = (self.orientation + 1) % 4
            self.commands.Forward()
            self.commands.Look()

    def search_line(self):
        tile_count, boundary, line_number = 0, 0, 1
        for tile in self.look:
            if "self" in tile:
                while boundary < tile_count:
                    boundary = line_number * 2 + 1
                    line_number += 1
            tile_count += 1
        return line_number

    def search_self_index(self):
        for i, tile in enumerate(self.look):
            if "self" in tile:
                return i
        return -1

    def get_triangle_indices(self):
        line = self.search_line()
        index = self.search_self_index()
        if index == -1:
            return
        if "self" in self.look[index]:
            self.look[index].remove("self")
        new_index = index
        if self.orientation == 0:
            new_index = index + 2 * line
        elif self.orientation == 1:
            new_index = index + 1
        elif self.orientation == 2:
            new_index = index - 2 * line
        elif self.orientation == 3:
            new_index = index - 1
        if 0 <= new_index < len(self.look):
            self.look[new_index].append("self")
        else:
            print(f"Warning: New position index {new_index} out of bounds")

    def find_resource_path(self, resource_name: str):
        coords = self.get_tile_relative_coords()
        print(f"Looking for {resource_name}...")
        closest_idx = None
        min_distance = float('inf')
        print(f"Titles contents: {self.look}", flush=True)
        for idx, tile in enumerate(self.look):
            # print(f"Item {idx}: {tile}")
            if resource_name in tile:
                if idx == 0:
                    # print(f"Found {resource_name} on our current tile")
                    return ["Take " + resource_name]
                x, y = coords[idx]
                distance = abs(x) + abs(y)
                print(f"Found {resource_name} at ({x}, {y}) with distance {distance}")
                if distance < min_distance:
                    min_distance = distance
                    closest_idx = idx
        if closest_idx is not None:
            x, y = coords[closest_idx]
            print(f"Closest {resource_name} is at ({x}, {y}) with distance {min_distance}")
            path = self.plan_cardinal_path_to((x, y), resource_name)
            if path:
                print(f"Path to closest {resource_name}: {path}")
                return path
            else:
                print(f"Failed to calculate path to closest {resource_name}", flush=True)
        # print(f"No {resource_name} found in visible tiles")
        return []

    def get_tile_relative_coords(self):
        """Generate relative (dx, dy) for each tile in self.look"""
        coords = [(0, 0)]
        for depth in range(1, self.level + 2):
            for i in range(-depth, depth + 1):
                coords.append((i, -depth))
        return [self.rotate_vector(dx, dy) for dx, dy in coords]

    def rotate_vector(self, dx, dy):
        """Rotate (dx, dy) according to orientation"""
        for _ in range(self.orientation):
            dx, dy = -dy, dx
        return dx, dy

    def plan_cardinal_path_to(self, target, resource_name=None):
        """Generate a series of commands to reach the target coordinates"""
        x, y = target
        path = []
        if self.orientation == 0:
            if y < 0:
                path = ["Forward"] * abs(y)
            elif y > 0:
                path = ["Right", "Right"] + ["Forward"] * y
            if x > 0:
                if not path:
                    path = ["Right"] + ["Forward"] * x
                else:
                    path += ["Right"] + ["Forward"] * x
            elif x < 0:
                if not path:
                    path = ["Left"] + ["Forward"] * abs(x)
                else:
                    path += ["Left"] + ["Forward"] * abs(x)
        elif self.orientation == 1:
            if x > 0:
                path = ["Forward"] * x
            elif x < 0:
                path = ["Right", "Right"] + ["Forward"] * abs(x)
            if y > 0:
                if not path:
                    path = ["Right"] + ["Forward"] * y
                else:
                    path += ["Right"] + ["Forward"] * y
            elif y < 0:
                if not path:
                    path = ["Left"] + ["Forward"] * abs(y)
                else:
                    path += ["Left"] + ["Forward"] * abs(y)
        if path and resource_name:
            path.append(f"Take {resource_name}")
        # print(f"Generated path: {path}")
        return path

    def set_dead(self):
        """Set the AI as dead and perform proper cleanup"""
        with self.lock:
            print("AI DEATH DETECTED - SHUTTING DOWN")
            self.running = False
            try:
                if self.connexion and self.connexion.socket:
                    self.connexion.socket.close()
                    self.connexion.connected = False
                    print("Socket closed due to death")
            except Exception as e:
                print(f"Error closing socket: {e}")
