##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## network
##

import asyncio
from typing import Optional
from sys import stderr
from asyncio import StreamReader, StreamWriter, Queue, Task

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

        self._incoming: Queue[str] = asyncio.Queue(10)
        self._outgoing: Queue[str] = asyncio.Queue(10)

        self._reader_task: Optional[Task[None]] = None
        self._writer_task: Optional[Task[None]] = None

    async def connect(self) -> None:
        """Connect to the Zappy server"""
        try:
            self.reader, self.writer = await asyncio.wait_for(
                asyncio.open_connection(self.hostname, self.port),
                timeout=5.0
            )
            self._connected = True

            self._reader_task = asyncio.create_task(self._background_reader())
            self._writer_task = asyncio.create_task(self._background_writer())

        except Exception as e:
            raise ConnectionError(
                f"Failed to connect to {self.hostname}:{self.port}: {e}"
            )

    async def disconnect(self) -> None:
        """Disconnect from the Zappy server"""
        self._connected = False

        if self._reader_task:
            self._reader_task.cancel()
            try:
                await self._reader_task
            except asyncio.CancelledError:
                pass

        if self._writer_task:
            self._writer_task.cancel()
            try:
                await self._writer_task
            except asyncio.CancelledError:
                pass

        if self.writer:
            self.writer.close()
            await self.writer.wait_closed()

        self.reader = None
        self._reader_task = None
        self.writer = None
        self._writer_task = None

    async def _background_reader(self) -> None:
        """Background task that continuously reads from the socket"""
        buffer = ""

        while self._connected:
            try:
                if not self.reader:
                    break

                data = await self.reader.read(4096)

                if not data:
                    self._connected = False
                    break

                buffer += data.decode("ascii")

                while '\n' in buffer:
                    line, buffer = buffer.split("\n", 1)
                    if line.strip():
                        await self._incoming.put(line.strip())
            except asyncio.CancelledError:
                print("Reader task cancelled")
                break
            except Exception as e:
                print(f"Error in background reader: {e}", file=stderr)
                self._connected = False
                break

    async def _background_writer(self) -> None:
        """Background task that continuously writes to the socket"""
        while self._connected:
            try:
                message: str = await self._outgoing.get()

                if not self.writer:
                    break

                if not message.endswith("\n"):
                    message += '\n'

                self.writer.write(message.encode("ascii"))
                await self.writer.drain()
            except asyncio.CancelledError:
                print("Writer task cancelled")
                break
            except Exception as e:
                print(f"Error in background writer: {e}", file=stderr)
                self._connected = False
                break

    def send(self, message: str) -> bool:
        """Queue a message to be sent"""
        if not self._connected:
            raise ConnectionError("Not connected to the server")

        try:
            self._outgoing.put_nowait(message)
            return True
        except asyncio.QueueFull:
            print("Send failed: message queue is full")
            return False
        except asyncio.QueueShutDown:
            print("The send queue was shut down", file=stderr)
            self._connected = False
            return False

    async def receive(self, blocking: bool = True) -> Optional[str]:
        """Receive a single message from the server"""
        if not self._connected:
            if blocking:
                raise ConnectionError(
                    "Not connected to the server"
                    )
            print("Not connected to the server")
            return None

        try:
            if blocking:
                return await self._incoming.get()
            else:
                return self._incoming.get_nowait()
        except asyncio.QueueEmpty:
            return None
        except asyncio.QueueShutDown:
            self._connected = False
            if blocking:
                raise ConnectionError(
                    "There is no more message to read " \
                    "(the receive queue was shut down)"
                )
            print("There is no more message to read " \
                  "(the receive queue was shut down)", file=stderr)
            return None

    def receive_all(self) -> list[str]:
        """Receive all pending messages (do not block)"""
        messages: list[str] = []

        while not self._incoming.empty():
            messages.append(self._incoming.get_nowait())
        return messages

    @property
    def is_connected(self) -> bool:
        """Check if the connection to the server is present"""
        return (self._connected and
                self.writer is not None and
                not self.writer.is_closing())
