#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## ai generator
##

import os
import sys
from connections import Connexion
import time
import signal
import atexit

class AIGenerator:
    def __init__(self, port, name, machine):
        self.port = port
        self.name = name
        self.machine = machine
        self.child_pids = []
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

    def fork(self, port, name, machine):
        pid = os.fork()
        if pid == 0:
            signal.signal(signal.SIGINT, signal.SIG_DFL)
            signal.signal(signal.SIGTERM, signal.SIG_DFL)
            connexion = Connexion(port, name, machine)
            if connexion.connect():
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
            try:
                pid_result, status = os.waitpid(pid, os.WNOHANG)
                if pid_result == 0:
                    print(f"Waiting for child {pid} to exit...")
                    os.waitpid(pid, 0)
                self.child_pids.remove(pid)
                print(f"Child process {pid} cleaned up")
            except ChildProcessError:
                self.child_pids.remove(pid)
                pass
        print("All child processes cleaned up.")
