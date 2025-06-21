#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## class to connect new IA to the server
##

from typing import Optional
import socket
import threading
import time
import os

"""This module provides a class to manage connections for AI clients in the Zappy game server.
It includes methods for connecting to the server, sending messages, and handling connection states.
"""
class Connection:
    def __init__(self, port: int, name: str, machine: str):
        self.port = port
        self.name = name
        self.machine = machine
        self.socket: Optional[socket.socket] = None
        self.connected = False
        self.buffer = ""
        self.waiting_for_response = False
        self.socket_lock = threading.Lock()

    """@brief Connects to the Zappy server.
        Checks if the connection is already established, and if not,
        attempts to create a new socket connection to the specified machine and port.
        If the connection was lost, it tries to reconnect.
        @return True if connection is successful, False otherwise.
        """
    def connect(self):
        if self.connected and self.socket:
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
    def send(self, message: str):
        if not self.connected or not self.socket:
            print("Not connected to the server.")
            return False
        
        with self.socket_lock:
            try:
                if not message.endswith('\n'):
                    message += '\n'
                self.socket.send(message.encode('ascii'))
                return True
            except BrokenPipeError:
                print("Connection broken (Broken pipe)")
                self.connected = False
                return False
            except Exception as e:
                print(f"Send error: {e}")
                return False

    """@brief Receives a complete message from the server.
        Uses a buffer to handle partial messages and returns complete lines.
        @param buffer_size: The size of the buffer to read data from the socket.
        @return The received complete message as a string, or None if an error occurs.
        """
    def receive(self, buffer_size: int = 1024, timeout: float = 5.0):
        if not self.connected or not self.socket:
            print("Not connected to the server.")
            return None
        
        try:
            # Set a timeout for blocking operations
            self.socket.settimeout(timeout)
            
            # Wait for a complete line with timeout
            start_time = time.time()
            while '\n' not in self.buffer and time.time() - start_time < timeout:
                try:
                    data = self.socket.recv(buffer_size).decode('ascii', errors='replace')
                    if not data:  # Connection closed by server
                        self.connected = False
                        return None
                        
                    self.buffer += data
                    
                    # Check for death message
                    if "dead" in data:
                        print("Death message detected in receive")
                        self.connected = False
                        return "dead"
                except socket.timeout:
                    # Timeout on recv is expected
                    continue
                except Exception as e:
                    print(f"Receive error: {e}")
                    return None
            
            # Extract the first line
            if '\n' in self.buffer:
                line, self.buffer = self.buffer.split('\n', 1)
                return line
            else:
                # Timeout without complete line
                return None
        finally:
            # Reset timeout to default
            self.socket.settimeout(None)

    def check_connection(self):
        """Check if connection is still valid and terminate if not"""
        if not self.connected:
            print("\n!!! CONNECTION LOST - TERMINATING AI !!!\n")
            os._exit(0)  # Force immediate termination
