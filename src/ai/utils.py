##
## EPITECH PROJECT, 2025
## zappy
## File description:
## utils
##

import threading
import time
import socket
# import queue
# import os
from loop import Loop

class ContinuousMonitor(threading.Thread):
    """Thread dedicated to continuously monitoring ticks and receiving messages"""
    def __init__(self, ai_instance: Loop):
        threading.Thread.__init__(self)
        self.daemon = True  # Thread will exit when main program exits
        self.ai_instance = ai_instance
        self.connexion = ai_instance.connexion
        self.running = True
        self.tick_counter = 0
        self.last_food_tick = 0
        self.socket_lock = threading.Lock() if not hasattr(self.connexion, 'socket_lock') else self.connexion.socket_lock
        self.start_time = time.time()
        self.ticks_per_second = 72 #value to change handly for the moment

    def run(self):
        """Main thread execution loop"""
        # print("Continuous monitor started")
        while self.running:
            try:
                if not self.connexion.connected:
                    # print("\n!!! CONNECTION LOST - TERMINATING AI !!!\n")
                    import os
                    os._exit(0)
                current_time = time.time()
                elapsed_seconds = current_time - self.start_time
                new_tick = int(elapsed_seconds * self.ticks_per_second)
                if new_tick > self.tick_counter:
                    old_tick = self.tick_counter
                    self.tick_counter = new_tick
                    self.ai_instance.tick_counter = new_tick
                    new_food_period = new_tick // 126
                    old_food_period = old_tick // 126
                    if new_food_period > old_food_period:
                        self.consume_food()
                self.check_for_messages()
                time_per_tick = 1.0 / self.ticks_per_second
                sleep_time = min(time_per_tick / 2, 0.05)
                time.sleep(sleep_time)
            except Exception as e:
                # print(f"Error in monitor thread: {e}")
                time.sleep(0.1)
        # print("Continuous monitor stopped")

    def consume_food(self):
        """Consume food every 126 ticks"""
        try:
            food_count = 0
            if hasattr(self.ai_instance, 'inventory'):
                food_count = self.ai_instance.inventory.get('food', 0)
            if food_count > 0:
                if hasattr(self.ai_instance, 'inventory_lock'):
                    with self.ai_instance.inventory_lock:
                        self.ai_instance.inventory['food'] = max(0, self.ai_instance.inventory['food'] - 1)
                else:
                    self.ai_instance.inventory['food'] = max(0, self.ai_instance.inventory['food'] - 1)
                new_count = self.ai_instance.inventory.get('food', 0)
                print(f"FOOD CONSUMED! Tick {self.tick_counter}. Previous: {food_count}, New: {new_count}", flush=True)
            else:
                print(f"No food to consume at tick {self.tick_counter}. Player may starve!")
        except Exception as e:
            print(f"Error consuming food: {e}")

    def decrypt_message(self, encrypted_message: str, name: str):
                    """
                    Decrypt a message that was encrypted with the encrypt_message method.
                    Uses the reverse of the encryption algorithm.
                    """
                    if not name:
                        return encrypted_message
                    shift = sum(ord(c) for c in name) % 26
                    decrypted: list[str] = []
                    for char in encrypted_message:
                        if char.isalpha():
                            ascii_offset = ord('a') if char.islower() else ord('A')
                            decrypted_char = chr((ord(char) - ascii_offset - shift) % 26 + ascii_offset)
                            decrypted.append(decrypted_char)
                        else:
                            decrypted.append(char)

                    return ''.join(decrypted)

    def check_for_messages(self):
        """Check for incoming messages without blocking"""
        if not self.connexion.connected:
            return
        if hasattr(self.connexion, 'waiting_for_response') and self.connexion.waiting_for_response:
            return
        if not self.socket_lock.acquire(blocking=False):
            return
        try:
            socket_copy = self.connexion.socket
            if not socket_copy:
                return
            try:
                was_blocking = socket_copy.getblocking()
                socket_copy.setblocking(False)
                try:
                    data = socket_copy.recv(1024)
                    if data:
                        decoded_data = data.decode('ascii', errors='replace')
                        # print(f"Monitor received raw data: {decoded_data}")
                        decoded_data = self.decrypt_message(decoded_data, self.connexion.name)
                        # print(f"Monitor decrypted data: {decoded_data}")
                        self.process_received_data(decoded_data)
                except (socket.error, BlockingIOError) as e:
                    if "Resource temporarily unavailable" not in str(e) and "Errno 11" not in str(e):
                        print(f"Socket recv error: {e}")
                except Exception as e:
                    print(f"Error receiving data: {e}")
                finally:
                    if was_blocking:
                        socket_copy.setblocking(True)
            except Exception as e:
                print(f"Socket access error: {e}")
        finally:
            self.socket_lock.release()

    def process_received_data(self, data: str):
        """Process incoming data from the server"""
        if "dead" in data:
            # print("DEATH DETECTED BY MONITOR: 'dead' found in received data")
            self.handle_death()
            return
        self.connexion.buffer += data
        lines = self.connexion.buffer.split('\n')
        self.connexion.buffer = lines[-1]
        for line in lines[:-1]:
            if line.strip():
                self.process_message(line.strip())

    def process_message(self, message: str):
        """Process a complete message"""
        # print(f"Monitor processing message: {message}")
        if message == "dead":
            # print("DEATH DETECTED BY MONITOR: direct 'dead' message")
            self.handle_death()
            return
        if message.startswith("message"):
            parts = message.split(' ', 2)
            if len(parts) >= 3:
                direction = parts[1]
                content = parts[2]
                self.handle_broadcast(direction, content)

    def handle_death(self):
        """Handle death detection"""
        print("PLAYER IS DEAD - SHUTTING DOWN")
        self.running = False
        if hasattr(self.ai_instance, 'running'):
            self.ai_instance.running = False
        if hasattr(self.ai_instance, 'is_dead'):
            self.ai_instance.is_dead = True
        try:
            if self.connexion and self.connexion.socket:
                self.connexion.socket.shutdown(socket.SHUT_RDWR)
                self.connexion.socket.close()
                self.connexion.connected = False
                print("Socket closed due to death")
        except Exception as e:
            print(f"Error closing socket: {e}")
        print("Terminating process due to player death")
        import os, signal
        os.kill(os.getpid(), signal.SIGTERM)