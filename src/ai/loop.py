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
        self.orientation = 0
        self.map_size = None
        self.commands = Commands(connexion)
        self.level = 1
        self.time_look = 9

    def run(self, map_size=None):
        if not self.connexion.connected:
            print("Failed to connect. Exiting loop.")
            return

        self.map_size = map_size
        if map_size:
            self.position = (0, 0)
        while self.running:
            try:
                if not self.connexion.connected:
                    print("Connection lost. Attempting to reconnect...")
                    if not self.connexion.connect():
                        print("Reconnection failed. Exiting loop.")
                        break
                self.make_decision()
                print("\nLook content:")
                print(f"Type: {type(self.look)}")
                if self.look:
                    for idx, item in enumerate(self.look):
                        print(f"Item {idx}: {item}")
                else:
                    print("Look is empty")
            except KeyboardInterrupt:
                print("\nExiting loop.")
                self.running = False


    def make_decision(self):
        self.time_look += 1
        if self.time_look == 10:
            self.look = self.commands.Look()
            print("Look command executed")
            self.time_look = 0
        if self.commands.Forward():
            print("Forward command executed")
            self.get_triangle_indices()
        elif self.commands.Right():
            self.orientation = (self.orientation + 1) % 4
            pass
        elif self.commands.Left():
            self.orientation = (self.orientation - 1) % 4
            pass
        else:
            return

    def search_line(self):
        a, b, c = 0, 0, 1
        for tile in self.look:
            if "self" in tile:
                while b < a:
                    b = c * 2 + 1
                    c += 1
            a += 1
        return c

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



