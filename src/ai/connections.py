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
class Connexion:
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
        import socket
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
            print("Not connected to the server.")
            return False
        try:
            self.socket.sendall(message.encode('utf-8'))
            return True
        except socket.error as e:
            print(f"Send error: {e}")
            return False

    """@brief Receives a message from the server.
        Checks if the connection is established before attempting to receive a message.
        If the connection is lost, it returns None.
        @param buffer_size: The size of the buffer to read data from the socket.
        @return The received message as a string, or None if an error occurs.
        """
    def receive(self, buffer_size=1024):
        if not self.connected:
            print("Not connected to the server.")
            return None
        try:
            data = self.socket.recv(buffer_size)
            return data.decode('utf-8')
        except socket.error as e:
            print(f"Receive error: {e}")
            return None

# def main():
#     port = 4444
#     name = "AI_Bot"
#     machine = "localhost"
#     connexion = Connexion(port, name, machine)
#     while connexion.connect():
#         # if connexion.send("Hello, server!"):
#             # print("Message sent successfully.")
#         # else:
#             # print("Failed to send message.")
#         response = connexion.receive()
#         if response:
#             print(f"Received from server: {response}")
#     else:
#         print("Failed to connect.")

# if __name__ == "__main__":
#     main()