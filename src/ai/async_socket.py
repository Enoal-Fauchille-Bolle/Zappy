##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## async_socket
##

import asyncio
from typing import Optional
from asyncio import StreamReader, StreamWriter

class AsyncSocket:
    """
    This module provides a class to manage connections for AI clients in the Zappy game server.
    It includes methods for connecting to the server, sending messages, and handling connection states.
    """

    def __init__(self, address: str, port: int):
        self.adrress = address
        self.port = port
        self.reader: Optional[StreamReader] = None
        self.writer: Optional[StreamWriter] = None
        self._connected = False
        self._receive_buffer = ""

    async def connect(self) -> None:
        try:
            self.reader, self.writer = await asyncio.open_connection(
                self.adrress, self.port
            )
            self._connected = True
        except Exception as e:
            raise ConnectionError(f"Failed to connect to {self.adrress}:{self.port}: {e}")

    async def disconnect(self) -> None:
        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()
        self._connected = False
        self.reader = None
        self.writer = None

    async def send(self, message: str) -> bool:
        if not self._connected:
            raise ConnectionError("Not connected to the server.")

        if not message.endswith('\n'):
            message += '\n'

        self.writer.write(message.encode("ascii"))
        await self.writer.drain()

    async def receive(self, blocking: bool = True) -> Optional[str]:
        if not self._connected:
            raise ConnectionError("Not connetc to the server.")

        if not blocking:
            return await self._receive_non_blocking()
        else:
            return await self._receive_blocking()

    async def _receive_blocking(self) -> str:
        while '\n' not in self._receive_buffer:
            data = await self.reader.read(4096)
            if not data:
                raise ConnectionError("Connection closed by the server")
            self._receive_buffer += data.decode("ascii")

        newline_pos = self._receive_buffer.find('\n')
        command = self._receive_buffer[:newline_pos + 1]
        self._receive_buffer = self._receive_buffer[newline_pos + 1:]

        return command

    async def _receive_non_blocking(self) -> Optional[str]:
        if '\n' in self._receive_buffer:
            newline_pos = self._receive_buffer.find('\n')
            command = self._receive_buffer[:newline_pos + 1]
            self._receive_buffer = self._receive_buffer[newline_pos + 1:]
            return command

        try:
            data = await asyncio.wait_for(self.reader.read(4096), timeout=0.001)
            if data:
                self._receive_buffer += data.decode("ascii")
                if '\n' in self._receive_buffer:
                    newline_pos = self._receive_buffer.find('\n')
                    command = self._receive_buffer[:newline_pos + 1]
                    self._receive_buffer = self._receive_buffer[newline_pos + 1:]
                    return command
        except asyncio.TimeoutError:
            pass
        return None

    @property
    def is_connected(self) -> bool:
        return self._connected

    async def __aenter__(self):
        await self.connect()
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await self.disconnect()







async def main():
    try:
        async with AsyncSocket("localhost", 4444) as client:
            await client.send("a")
            reponse = await client.receive()
            reponse += await client.receive()
            reponse += await client.receive()
            print(f"Received: {reponse.strip()}")
    except ConnectionError as e:
        print(f"Connection error: {e}")

if __name__ == "__main__":
    asyncio.run(main())
