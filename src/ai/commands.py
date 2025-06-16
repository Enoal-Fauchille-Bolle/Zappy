##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Zappy commands handling
##

import time

class Commands:
    def __init__(self, connexion):
        self.connexion = connexion

    def Forward(self):
        if self.connexion.connected:
            self.connexion.send("Forward\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 128
        print(f"Forward elapsed time: {elapsed_time}")  # Debugging line
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True

    def Right(self):
        if self.connexion.connected:
            self.connexion.send("Right\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        print(f"Right elapsed time: {elapsed_time}")  # Debugging line
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True

    def Left(self):
        if self.connexion.connected:
            self.connexion.send("Left\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        print(f"Left elapsed time: {elapsed_time}")
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True

    def Look(self):
        if self.connexion.connected:
            self.connexion.send("Look\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return None
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        print(f"Look response: {response}") # Line added to respect binary protocol
        print(f"Look elapsed time: {elapsed_time}")  # Debugging line to see the elapsed time
        if response is None or response == "ko":
            return None
        if response == "ok":
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
        print(f"Look tiles: {tiles}")  # Debugging line to see the tiles
        if tiles and len(tiles) > 0 and "player" in tiles[0]:
            tiles[0].remove("player")
            tiles[0].append("self")
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.look = tiles
            ai.tick.append(elapsed_time)
            ai.tile = {
            "food": 0, "linemate": 0, "deraumere": 0,
            "sibur": 0, "mendiane": 0, "phiras": 0,
            "thystame": 0, "player": 0, "egg": 0, "self": 0
            }
        return tiles

    def Inventory(self):
        if self.connexion.connected:
            self.connexion.send("Inventory\n")
            start_time = time.time()
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 2
        print(f"Inventory response: {response}")  # Debugging line
        if response is None or response == "ko":
            return None
        inventory = {}
        items_str = response.strip()
        if items_str.startswith('[') and items_str.endswith(']'):
            items_str = items_str[1:-1]
        item_pairs = items_str.split(',')
        for pair in item_pairs:
            pair = pair.strip()
            parts = pair.split()
            if len(parts) >= 2:
                item_name = parts[0].strip()
                try:
                    item_count = int(parts[1].strip())
                    inventory[item_name] = item_count
                except ValueError:
                    print(f"Error parsing count for item: {item_name}")
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.inventory.update(inventory)
            print(f"Updated inventory: {ai.inventory}")
            ai.tick.append(elapsed_time)

        return inventory

    def Broadcast(self, message):
        if self.connexion.connected:
            start_time = time.time()
            self.connexion.send(f"Broadcast {message}\n")
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True

    def Connect_nbr(self):
        if self.connexion.connected:
            self.connexion.send("Connect_nbr\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return None
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time)
        print(f"Connect_nbr elapsed time: {elapsed_time}")  # Debugging line
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response is None or response == "ko":
            return None
        try:
            return int(response.strip())
        except ValueError:
            return None

    def Take(self, item):
        if self.connexion.connected:
            self.connexion.send(f"Take {item}\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        print(f"Take elapsed time: {elapsed_time}")  # Debugging line
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            if item in ai.inventory:
                ai.inventory[item] += 1
            else:
                ai.inventory[item] = 1
            if ai.look and len(ai.look) > 0 and item in ai.look[0]:
                ai.look[0].remove(item)
                print(f"Removed {item} from look[0] after taking it")
            print(f"Picked up {item}. Updated inventory: {ai.inventory}")
        return True

    def Set(self, item):
        if self.connexion.connected:
            self.connexion.send(f"Set {item}\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            if item in ai.inventory and ai.inventory[item] > 0:
                ai.inventory[item] -= 1
                if ai.look and len(ai.look) > 0:
                    ai.look[0].append(item)
                    print(f"Added {item} to look[0] after setting it")
                print(f"Dropped {item}. Updated inventory: {ai.inventory}")
            else:
                print(f"Warning: Dropped {item} but wasn't in local inventory")
        return True

    def Incantation(self):
        if self.connexion.connected:
            self.connexion.send("Incantation\n")
        else:
            print("Not connected to the server.")
            return False
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
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        if response == "ko":
            return False
        from ai_generator import AIGenerator  # avoid circular import
        ai_gen = AIGenerator(self.connexion.port, self.connexion.name, self.connexion.machine)
        ai_gen.fork(self.connexion.port, self.connexion.name, self.connexion.machine)
        return True

    def Eject(self):
        if self.connexion.connected:
            start_time = time.time()
            self.connexion.send("Eject\n")
        else:
            print("Not connected to the server.")
            return False
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 7
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True