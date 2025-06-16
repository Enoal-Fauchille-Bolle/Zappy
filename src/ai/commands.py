##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Zappy commands handling
##

class Commands:
    def __init__(self, connexion):
        self.connexion = connexion

    def Forward(self):
        if self.connexion.connected:
            self.connexion.send("Forward\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        return True

    def Right(self):
        if self.connexion.connected:
            self.connexion.send("Right\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        return True

    def Left(self):
        if self.connexion.connected:
            self.connexion.send("Left\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        return True

    def Look(self):
        if self.connexion.connected:
            self.connexion.send("Look\n")
        response = self.connexion.receive()
        print(f"Look response: {response}") # Line added to respect binary protocol
        if response is None or response == "ko":
            return None
        response = self.connexion.receive()
        print(f"Look response: {response}")
        if response.startswith('[') and response.endswith(']'):
            response = response[1:-1]
        tiles = []
        if not response.strip():
            return []
        raw_tiles = response.split(',')
        for tile in raw_tiles:
            tile_objects = [obj for obj in tile.strip().split() if obj]
            tiles.append(tile_objects)
        if tiles and len(tiles) > 0 and "player" in tiles[0]:
            tiles[0].remove("player")
            tiles[0].append("self")
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.look = tiles
            ai.tile = {
            "food": 0, "linemate": 0, "deraumere": 0,
            "sibur": 0, "mendiane": 0, "phiras": 0,
            "thystame": 0, "player": 0, "egg": 0, "self": 0
            }
        return tiles

    def Inventory(self):
        if self.connexion.connected:
            self.connexion.send("Inventory\n")
        response = self.connexion.receive()
        if response is None or response == "ko":
            return None
        inventory = {}
        items_str = response.strip()
        if items_str.startswith('[') and items_str.endswith(']'):
            items_str = items_str[1:-1]
        items = items_str.split()
        for item in items:
            item = item.strip(',')
            if item in inventory:
                inventory[item] += 1
            else:
                inventory[item] = 1
        return inventory

    def Broadcast(self, message):
        if self.connexion.connected:
            self.connexion.send(f"Broadcast {message}\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        return True

    def Connect_nbr(self):
        if self.connexion.connected:
            self.connexion.send("Connect_nbr\n")
        response = self.connexion.receive()
        if response is None or response == "ko":
            return None
        try:
            return int(response.strip())
        except ValueError:
            return None

    def Take(self, item):
        if self.connexion.connected:
            self.connexion.send(f"Take {item}\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            inventory = self.connexion.ai_instance.inventory
            if item in inventory:
                inventory[item] += 1
            else:
                inventory[item] = 1
            print(f"Picked up {item}. Updated inventory: {inventory}")
        return True

    def Set(self, item):
        if self.connexion.connected:
            self.connexion.send(f"Set {item}\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            inventory = self.connexion.ai_instance.inventory
            if item in inventory and inventory[item] > 0:
                inventory[item] -= 1
                print(f"Dropped {item}. Updated inventory: {inventory}")
            else:
                print(f"Warning: Dropped {item} but wasn't in local inventory")
        return True

    def Incantation(self):
        if self.connexion.connected:
            self.connexion.send("Incantation\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        response = self.connexion.receive()
        if response is None or response == "ko":
            return False
        try:
            return int(response.strip())
        except ValueError:
            return False

    def Fork(self):
        if self.connexion.connected:
            self.connexion.send("Fork\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        from ai_generator import AIGenerator  # avoid circular import
        ai_gen = AIGenerator(self.connexion.port, self.connexion.name, self.connexion.machine)
        ai_gen.fork(self.connexion.port, self.connexion.name, self.connexion.machine)
        return True

    def Eject(self):
        if self.connexion.connected:
            self.connexion.send("Eject\n")
        response = self.connexion.receive()
        if response == "ko":
            return False
        return True