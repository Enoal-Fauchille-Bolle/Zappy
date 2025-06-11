##
## EPITECH PROJECT, 2025
## zappy
## File description:
## AI loop for Zappy
##

from commands import Commands

class Loop:
    def __init__(self, connexion):
        self.connexion = connexion
        self.running = True
        self.inventory = {"food": 0, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0}
        self.look = []
        self.position = None
        self.orientation = 1
        self.map_knowledge = {}
        self.map_size = None
        self.commands = Commands(connexion)

    def run(self, map_size=None):
        if not self.connexion.connected:
            print("Failed to connect. Exiting loop.")
            return

        self.map_size = map_size
        print(f"Starting AI loop with map size: {map_size}")
        if map_size:
            self.position = (0, 0)
        while self.running:
            try:
                if not self.connexion.connected:
                    print("Connection lost. Attempting to reconnect...")
                    if not self.connexion.connect():
                        print("Reconnection failed. Exiting loop.")
                        break
                self.update_surroundings()
                self.make_decision()
            except KeyboardInterrupt:
                print("\nExiting loop.")
                self.running = False

    def update_surroundings(self):
        look_result = self.commands.Look()
        if look_result:
            self.look = look_result
            self.update_map_from_look()
        inventory_result = self.commands.Inventory()
        if inventory_result:
            self.inventory = inventory_result
        print(f"Position: {self.position}, Orientation: {self.get_orientation_name()}")
        print(f"Inventory: {self.inventory}")

    def update_map_from_look(self):
        if not self.look:
            return
        self.map_knowledge = {}
        self.map_knowledge[self.position] = self.parse_tile_content(self.look[0])
        tile_index = 1
        vision_level = 1
        while tile_index < len(self.look):
            tiles_in_level = 2 * vision_level - 1
            for offset in range(-(tiles_in_level // 2), (tiles_in_level // 2) + 1):
                if tile_index >= len(self.look):
                    break
                rel_pos = None
                if self.orientation == 1:
                    rel_pos = (offset, -vision_level)
                elif self.orientation == 2:
                    rel_pos = (vision_level, offset)
                elif self.orientation == 3:
                    rel_pos = (-offset, vision_level)
                elif self.orientation == 4:
                    rel_pos = (-vision_level, -offset)
                if rel_pos:
                    abs_pos = self.calculate_absolute_position(rel_pos)
                    self.map_knowledge[abs_pos] = self.parse_tile_content(self.look[tile_index])
                tile_index += 1
            vision_level += 1

    def parse_tile_content(self, content):
        resources = {}
        if not content:
            return resources
        items = content.split()
        for item in items:
            item = item.strip()
            if item:
                if item in resources:
                    resources[item] += 1
                else:
                    resources[item] = 1
        return resources

    def calculate_absolute_position(self, rel_pos):
        if not self.position:
            return None
        x, y = self.position
        dx, dy = rel_pos
        new_x = x + dx
        new_y = y + dy
        if self.map_size:
            width, height = self.map_size
            new_x = new_x % width
            new_y = new_y % height
        return (new_x, new_y)

    def make_decision(self):
        if self.commands.Forward():
            self.update_position_after_move()
        else:
            print("Forward movement failed")
            self.commands.Right()
            self.update_orientation_after_turn("right")

    def update_position_after_move(self):
        if self.position is None:
            return
        x, y = self.position
        if self.orientation == 1:
            self.position = (x, y-1)
        elif self.orientation == 2:
            self.position = (x+1, y)
        elif self.orientation == 3:
            self.position = (x, y+1)
        elif self.orientation == 4:
            self.position = (x-1, y)
        if self.map_size:
            width, height = self.map_size
            x, y = self.position
            self.position = (x % width, y % height)
        print(f"Position updated to {self.position}")

    def update_orientation_after_turn(self, direction):
        if direction == "right":
            self.orientation = (self.orientation % 4) + 1
        elif direction == "left":
            self.orientation = ((self.orientation - 2) % 4) + 1
        print(f"Orientation updated to {self.get_orientation_name()}")

    def get_orientation_name(self):
        orientations = {1: "North", 2: "East", 3: "South", 4: "West"}
        return orientations.get(self.orientation, "Unknown")
