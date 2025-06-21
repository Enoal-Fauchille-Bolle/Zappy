#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## ai generator
##

import os
import sys
from connexions import Connection
from loop import Loop
import signal
import atexit

class AIGenerator:
    def __init__(self, port: int, name: str, machine: str):
        self.port = port
        self.name = name
        self.machine = machine
        self.child_pids: list[int] = []
        signal.signal(signal.SIGINT, self.handle_signal)
        signal.signal(signal.SIGTERM, self.handle_signal)
        atexit.register(self.clean_up)

    def handle_signal(self, sig, frame):
        print("\nShutting down AI Generator...")
        self.terminate_children()
        sys.exit(0)

    def terminate_children(self):
        for pid in self.child_pids:
            try:
                os.kill(pid, signal.SIGTERM)
                print(f"Sent SIGTERM to child process {pid}")
            except ProcessLookupError:
                pass

    def generate_ai(self):
        self.fork(self.port, self.name, self.machine)

    """!@brief Re-forks the AI process based on the server's response.
        This method checks if the connection is still active and processes the server's response.
        If the response indicates a positive number, it forks a new AI process.
        If the response contains map dimensions, it prints them.
        @param port: The port to connect to the server.
        @param name: The name of the AI.
        @param machine: The machine address of the server.
        @param connection: The Connection object used to communicate with the server.
        """
    def re_fork(self, port: int, name: str, machine: str, connection: Connection):
        if connection.connected:
            client_number = connection.receive()
            print(f"Server response received (client number): {client_number}")
            if client_number is not None:
                try:
                    num = int(client_number.strip())
                    print(f"Server returned client number: {num}")
                    map_size_response = connection.receive()
                    print(f"Server response received (map size): {map_size_response}")
                    map_size = None
                    if map_size_response and map_size_response.strip():
                        dimensions = map_size_response.strip().split()
                        if len(dimensions) >= 2:
                            map_width = int(dimensions[0])
                            map_height = int(dimensions[1])
                            map_size = (map_width, map_height)
                            print(f"Map size: {map_width}x{map_height}")
                    connection.map_size = map_size

                    if num > 0:
                        self.fork(port, name, machine)
                    else:
                        print(f"Received non-positive number: {num}, not spawning more AIs")
                except (ValueError, AttributeError, IndexError) as e:
                    print(f"Error parsing server response: {e}")

    """!@brief Forks a new AI process.
        This method creates a new child process for the AI.
        It sets up signal handling for the child process and establishes a connection to the server.
        If the connection is successful, it sends the AI's name to the server and waits for further instructions.
        @param port: The port to connect to the server.
        @param name: The name of the AI.
        @param machine: The machine address of the server.
        @return The PID of the forked process, or None if the fork failed.
        """
    def fork(self, port: int, name: str, machine: str):
        pid = os.fork()
        if pid == 0:
            signal.signal(signal.SIGINT, signal.SIG_DFL)
            signal.signal(signal.SIGTERM, signal.SIG_DFL)
            connection = Connection(port, name, machine)
            if connection.connect():
                welcome = connection.receive()
                print(f"Server says: {welcome}")
                
                # Actually send the team name to the server
                connection.send(name)  # Your Connection.send() already adds newlines as needed
                print(f"Sent team name: {name}")
                
                self.re_fork(port, name, machine, connection)
                ai_loop = Loop(connection)
                ai_loop.run(connection.map_size if hasattr(connection, 'map_size') else None)
            else:
                print("Failed to connect to the server.")
            os._exit(0)
        elif pid < 0:
            print("Fork failed.")
            return None
        else:
            self.child_pids.append(pid)
            print(f"Forked AI process with PID {pid} for {name} on {machine}:{port}")
            return pid

    def clean_up(self):
        """Wait for all child processes to finish"""
        print("Cleaning up child processes...")
        for pid in self.child_pids[:]:
            print(f"Waiting for child {pid} to exit...")
            os.waitpid(pid, 0)
            self.child_pids.remove(pid)
            print(f"Child process {pid} cleaned up")
        print("All child processes cleaned up.")
