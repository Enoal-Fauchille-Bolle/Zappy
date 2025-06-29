##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## generator
##

import multiprocessing as mp
import sys
from dataclasses import dataclass

@dataclass
class AIProcessInfo:
    """Information about an AI process"""
    process: mp.Process
    pid: int
    team_name: str

def run_ai_in_process(hostname: str, port: int, team_name: str) -> None:
    """
    Entry point for Ai processes.
    """
    ai = None
    try:
        from .client import ZappyAi
        import asyncio

        ai = ZappyAi(hostname, port, team_name)
        asyncio.run(ai.run())
    except KeyboardInterrupt:
        print(f"AI process {mp.current_process().pid} shutting down")
        if ai is not None:
            ai.process_manager.shutdown_all()
        sys.exit(0)
    except Exception as e:
        print(f"Error in Ai process {mp.current_process().pid}: {e}", file=sys.stderr)
        sys.exit(84)

class ProcessManager:
    """
    Manages Ai processes

    Each process is independent
    """

    def __init__(self) -> None:
        self._processes: dict[int, AIProcessInfo] = {}

    def spawn_ai_process(self, hostname: str, port: int,
                               team_name: str) -> int:
        """Spawn a new AI process."""
        try:
            process = mp.Process(
                target=run_ai_in_process,
                args=(hostname, port, team_name),
                daemon=False
            )
            process.start()

            if process.pid is None:
                raise RuntimeError("Failed to start new process")

            process_info = AIProcessInfo(
                process=process,
                pid=process.pid,
                team_name=team_name
            )
            self._processes[process.pid] = process_info

            print(f"Spawned AI process {process.pid} for team {team_name}")
            return process.pid
        except Exception as e:
            print(f"Failed to spawn AI process: {e}", file=sys.stderr)
            raise

    def shutdown_all(self) -> None:
        """Gracefully shutdown all AI processes."""
        if not self._processes:
            return

        for process_info in self._processes.values():
            if process_info.process.is_alive():
                from os import kill
                from signal import SIGINT
                kill(process_info.pid, SIGINT)
                # print(f"Calling terminate on {process_info.pid}")
                # process_info.process.terminate()

        for process_info in self._processes.values():
            process_info.process.join()
            process_info.process.close()

        self._processes.clear()

    @property
    def active_processes(self) -> dict[int, str]:
        """Get a dict of active process"""
        return {
            pid: info.team_name
            for pid, info in self._processes.items()
            if info.process.is_alive()
        }

def spawn_independent_ai(hostname: str, port: int, team_name: str) -> mp.Process:
    """
    Spawn a AI process without managment.
    """
    process = mp.Process(
        target=run_ai_in_process,
        args=(hostname, port, team_name),
        daemon=True
    )
    process.start()
    if process.pid is not None:
        print(f"Started process {process.pid}")
    return process
