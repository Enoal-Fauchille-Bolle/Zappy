##
## EPITECH PROJECT, 2025
## zappy
## File description:
## AI loop for Zappy
##

import random
from commands import Commands

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
        self.time_tick = 0
        self.food_tick = 0
        self.current_path = []

    def run(self, map_size=None):
        if not self.connexion.connected:
            # print("Failed to connect. Exiting loop.")
            return

        self.map_size = map_size
        if map_size:
            self.position = (0, 0)
        while self.running:
            try:
                if not self.connexion.connected:
                    # print("Connection lost. Attempting to reconnect...")
                    if not self.connexion.connect():
                        # print("Reconnection failed. Exiting loop.")
                        break
                self.make_decision()
                # print("\nLook content:")
                # print(f"Type: {type(self.look)}")
                self.commands.Connect_nbr()
                self.get_tick()
                # print(f"Time tick: {self.time_tick}")
                if self.look:
                    for idx, item in enumerate(self.look):
                        print(f"Item {idx}: {item}")
                else:
                    print("Look is empty")
            except KeyboardInterrupt:
                # print("\nExiting loop.")
                self.running = False

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
        self.check_and_update_look()
        if self.inventory["food"] < 3:
            if self.current_path:
                success = self.execute_next_command()
                if not success:
                    self.current_path = []
                return
            self.plan_next_action()
        else:
            return

    def check_and_update_look(self):
        """Check if it's time to perform a look action"""
        self.time_look += 1
        if self.time_look == 10:
            self.commands.Look()
            self.time_look = 0
            # print(f"Look content:")
            # print(f"Type: {type(self.look)}")

    def execute_next_command(self):
        """Execute the next command in the current path"""
        if not self.current_path:
            return True
        next_command = self.current_path.pop(0)
        # print(f"Executing command: {next_command}")
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

    def plan_next_action(self):
        """Plan the next action when there's no current path"""
        path = self.find_resource_path("food")
        if path:
            # print(f"Path to food: {path}")
            self.current_path = path
        else:
            # print("No path to food found, moving randomly")
            directions = ["Forward", "Right", "Left"]
            move = random.choice(directions)
            if move == "Forward":
                self.commands.Forward()
            elif move == "Right":
                self.commands.Right()
                self.orientation = (self.orientation + 1) % 4
            elif move == "Left":
                self.commands.Left()
                self.orientation = (self.orientation - 1) % 4

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
        # print(f"Looking for {resource_name}...")
        closest_idx = None
        min_distance = float('inf')
        for idx, tile in enumerate(self.look):
            # print(f"Item {idx}: {tile}")
            if resource_name in tile:
                if idx == 0:
                    # print(f"Found {resource_name} on our current tile")
                    return ["Take " + resource_name]
                x, y = coords[idx]
                distance = abs(x) + abs(y)
                # print(f"Found {resource_name} at ({x}, {y}) with distance {distance}")
                if distance < min_distance:
                    min_distance = distance
                    closest_idx = idx
        if closest_idx is not None:
            x, y = coords[closest_idx]
            # print(f"Closest {resource_name} is at ({x}, {y}) with distance {min_distance}")
            path = self.plan_cardinal_path_to((x, y), resource_name)
            if path:
                # print(f"Path to closest {resource_name}: {path}")
                return path
            else:
                print(f"Failed to calculate path to closest {resource_name}")
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

        # print(f"Planning path to ({x}, {y}) with orientation {self.orientation}")

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
