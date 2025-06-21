##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## network
##

import asyncio
from typing import Optional
from asyncio import StreamReader, StreamWriter

class NetworkManager:
    """
    This module provides a class to manage network communication
    with the Zappy game server.
    """

    def __init__(self, hostname: str, port: int) -> None:
        self.hostname = hostname
        self.port = port
        self.reader: Optional[StreamReader] = None
        self.writer: Optional[StreamWriter] = None
        self._connected: bool = False
        self._receive_buffer: str = ""

    async def connect(self) -> None:
        """Connect to the Zappy server"""
        try:
            self.reader, self.writer = await asyncio.wait_for(
                asyncio.open_connection(self.hostname, self.port),
                timeout=5.0
            )
            self._connected = True

        except Exception as e:
            raise ConnectionError(
                f"Failed to connect to {self.hostname}:{self.port}: {e}"
            )

    async def disconnect(self) -> None:
        """Disconnect from the Zappy server"""
        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()
        self._connected = False
        self.reader = None
        self.writer = None

    async def send(self, message: str):
        """Send a message to the server"""
        if not self._connected or not self.writer:
            raise ConnectionError(
                "Not connected to the server"
            )

        if not message.endswith('\n'):
            message += '\n'

        self.writer.write(message.encode("ascii"))
        await self.writer.drain()
        return True

    async def receive(self, timeout: Optional[float] = None) -> Optional[str]:
        """Receive a single message from the server"""
        if not self._connected or not self.reader:
            if timeout is None:
                raise ConnectionError(
                    "Not connected to the server"
                    )
            return None

        if '\n' in self._receive_buffer:
            line, self._receive_buffer = self._receive_buffer.split("\n", 1)
            return line.strip()

        try:
            if timeout is not None:
                data = await asyncio.wait_for(
                    self.reader.read(4096),
                    timeout=timeout
                )
            else:
                data = await self.reader.read(4096)

            if not data:
                self._connected = False
                if timeout is None:
                    raise ConnectionError("Connection closed by the server")
                return None
            self._receive_buffer += data.decode("ascii")

            if '\n' in self._receive_buffer:
                line, self._receive_buffer = self._receive_buffer.split("\n", 1)
                return line.strip()

            if timeout is None:
                return await self.receive(None)
        except asyncio.TimeoutError:
            pass
        return None

    async def receive_all(self) -> list[str]:
        """Receive all pending messages (do not block)"""
        messages: list[str] = []

        while '\n' in self._receive_buffer:
            line, self._receive_buffer = self._receive_buffer.split("\n", 1)
            if line.strip():
                messages.append(line.strip())

        if not self._connected or not self.reader:
            return messages
        try:
            data = await asyncio.wait_for(self.reader.read(4096), 0.001)
            if not data:
                self._connected = False
                return messages
            self._receive_buffer += data.decode("ascii")
            while '\n' in self._receive_buffer:
                line, self._receive_buffer = self._receive_buffer.split("\n", 1)
                if line.strip():
                    messages.append(line.strip())
        except asyncio.TimeoutError:
            pass
        return messages

    def is_connected(self) -> bool:
        """Check if the connection to the server is present"""
        return self._connected and  \
            self.writer is not None and not self.writer.is_closing()
