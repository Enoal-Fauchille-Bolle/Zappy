##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Zappy commands handling
##

import time
import sys
import os
from connexions import Connection
from loop import Loop

class Commands:
    def __init__(self, connexion: Connection):
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
        print(f"Forward elapsed time: {elapsed_time}", flush=True)  # Debugging line
        sys.stdout.flush()
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
        elapsed_time = (end_time - start_time) / 128
        print(f"Right elapsed time: {elapsed_time}", flush=True)  # Debugging line
        sys.stdout.flush()
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
        print(f"Left elapsed time: {elapsed_time}", flush=True)
        sys.stdout.flush()
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False
        return True

    def Look(self) -> None | list[list[str]]:
        if self.connexion.connected:
            self.connexion.send("Look\n")
            start_time = time.time()
        else:
            print("Not connected to the server.")
            return None
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 128
        # print(f"Look response: {response}") # Line added to respect binary protocol
        print(f"Look elapsed time: {elapsed_time}", flush=True)  # Debugging line to see the elapsed time
        if response is None or response == "ko":
            return None
        if response == "ok":
            response = self.connexion.receive()
        if response is None:
            return None
        # print(f"Look response: {response}")
        if response.startswith('[') and response.endswith(']'):
            response = response[1:-1]
        tiles: list[list[str]] = []
        if not response.strip():
            return []
        raw_tiles = response.split(',')
        for tile in raw_tiles:
            tile_objects = [obj for obj in tile.strip().split() if obj]
            tiles.append(tile_objects)
        # print(f"Look tiles: {tiles}")  # Debugging line to see the tiles
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
        inventory: dict[str, int] = {}
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

    def Broadcast(self, message: str):
        if not self.connexion.connected or not self.connexion.socket:
            print("Not connected to server, can't broadcast")
            return False
        try:
            encrypted_message = self.encrypt_message(message, self.connexion.name)
            print(f"Original message: {repr(message)}")
            print(f"Encrypted message: {repr(encrypted_message)}")
            self.connexion.waiting_for_response = True
            result = self.connexion.send(f"Broadcast {encrypted_message}\n")
            if not result:
                print("Send failed")
                self.connexion.waiting_for_response = False
                return False
            response = self.connexion.receive(timeout=2.0)
            self.connexion.waiting_for_response = False
            if response is None or response == "dead":
                print("Connection likely broken or AI is dead")
                if response == "dead":
                    self.handle_death(self.connexion.ai_instance)
                return False
            return response == "ok"
        except Exception as e:
            print(f"Broadcast error: {e}")
            self.connexion.waiting_for_response = False
            if "Broken pipe" in str(e):
                print("Connection broken - marking as disconnected")
                self.connexion.connected = False
            return False

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

    def Take(self, item: str):
        if self.connexion.connected:
            self.connexion.send(f"Take {item}\n")
        else:
            print("Not connected to the server.")
        start_time = time.time()
        response = self.connexion.receive()
        end_time = time.time()
        elapsed_time = (end_time - start_time) / 128
        print(f"Take elapsed time: {elapsed_time}")  # Debugging line
        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            ai.tick.append(elapsed_time)
        if response == "ko":
            return False

        if hasattr(self.connexion, 'ai_instance') and self.connexion.ai_instance:
            ai = self.connexion.ai_instance
            with ai.inventory_lock:
                if item in ai.inventory:
                    ai.inventory[item] += 1
                else:
                    ai.inventory[item] = 1
                with ai.lock:
                    if ai.look and len(ai.look) > 0 and item in ai.look[0]:
                        ai.look[0].remove(item)
                        print(f"Removed {item} from look[0] after taking it")
                print(f"Picked up {item}. Updated inventory: {ai.inventory}")
        return True

    def Set(self, item: str):
        if self.connexion.connected:
            self.connexion.send(f"Set {item}\n")
        else:
            print("Not connected to the server.")
        start_time = time.time()
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
            with ai.inventory_lock:
                if item in ai.inventory and ai.inventory[item] > 0:
                    ai.inventory[item] -= 1
                    with ai.lock:
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

    def encrypt_message(self, message: str, name: str):
        """
        Encrypt a message using a simple algorithm based on the name.
        This is a basic implementation and can be replaced with a more sophisticated one.
        """
        if not name:
            return message
        shift = sum(ord(c) for c in name) % 26
        encrypted: list[str] = []
        for char in message:
            if char.isalpha():
                ascii_offset = ord('a') if char.islower() else ord('A')
                encrypted_char = chr((ord(char) - ascii_offset + shift) % 26 + ascii_offset)
                encrypted.append(encrypted_char)
            else:
                encrypted.append(char)

        return ''.join(encrypted)

    def handle_death(self, ai_instance: Loop):
        """Handle the AI's death by terminating immediately"""
        print("\n!!! DEATH DETECTED IN COMMAND - TERMINATING !!!\n")
        
        # Force immediate exit
        os._exit(0)