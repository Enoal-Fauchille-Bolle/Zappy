#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## class to connect new IA to the server
##

import socket

"""This module provides a class to manage connections for AI clients in the Zappy game server.
It includes methods for connecting to the server, sending messages, and handling connection states.
"""
class Connection:
    def __init__(self, port, name, machine):
        self.port = port
        self.name = name
        self.machine = machine
        self.socket = None
        self.connected = False

    """@brief Connects to the Zappy server.
        Checks if the connection is already established, and if not,
        attempts to create a new socket connection to the specified machine and port.
        If the connection was lost, it tries to reconnect.
        @return True if connection is successful, False otherwise.
        """
    def connect(self):
        if self.connected:
            try:
                self.socket.getpeername()
                #print("Already connected.")
                return True
            except socket.error:
                print("Connection lost. Trying to reconnect.")
                self.connected = False
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.machine, self.port))
            self.connected = True
            print(f"Connected to {self.machine}:{self.port} as {self.name}")
            self.buffer = ""
        except socket.error as e:
            print(f"Connection error: {e}")
            self.connected = False
        return self.connected

    """@brief Sends a message to the server.
        Checks if the connection is established before attempting to send a message.
        If the connection is lost, it returns False.
        @param message: The message to be sent to the server.
        @return True if the message was sent successfully, False otherwise.
        """
    def send(self, message):
        if not self.connected:
            if not self.connect():
                print("Not connected to the server.")
                return False
            return self.send(message)
        try:
            message = message.strip() + "\n"
            self.socket.sendall(message.encode('ascii'))
            return True
        except socket.error as e:
            print(f"Send error: {e}")
            return False

    """@brief Receives a complete message from the server.
        Uses a buffer to handle partial messages and returns complete lines.
        @param buffer_size: The size of the buffer to read data from the socket.
        @return The received complete message as a string, or None if an error occurs.
        """
    def receive(self, buffer_size=1024):
        if not self.connected:
            print("Not connected to the server.")
            return None
        try:
            while '\n' not in self.buffer:
                data = self.socket.recv(buffer_size)
                if not data:
                    return None
                self.buffer += data.decode('ascii')
            lines = self.buffer.split('\n', 1)
            message = lines[0]
            self.buffer = lines[1] if len(lines) > 1 else ""
            return message
        except socket.error as e:
            print(f"Receive error: {e}")
            return None
