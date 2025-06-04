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

class AIGenerator:
    def __init__(self, port, name, machine):
        self.port = port
        self.name = name
        self.machine = machine
        self.ia_number = 0
        self.max_ai = 10
        self.child_pids = []
        signal.signal(signal.SIGINT, self.handle_signal)
        signal.signal(signal.SIGTERM, self.handle_signal)

    def handle_signal(self, sig, frame):
        print("\nShutting down AI Generator...")
        sys.exit(0)

    def generate_ai(self):
        connexion = Connexion(self.port, self.name, self.machine)
        if not connexion.connect():
            connexion.connect()
        self.fork(self.port, self.name, self.machine)

    def fork(self, port, name, machine):
        pid = os.fork()
        if pid == 0:
            signal.signal(signal.SIGINT, signal.SIG_DFL)
            signal.signal(signal.SIGTERM, signal.SIG_DFL)
            connexion = Connexion(port, name, machine)
            if connexion.connect():
                connexion.send(f"azerty")
                try:
                    while True:
                        time.sleep(5)
                        if not connexion.send("test"):
                            break
                except Exception as e:
                    print(f"AI has an error: {e}")
            else:
                print("Failed to connect to the server.")
            os._exit(0)
        elif pid < 0:
            print("Fork failed.")
            return None
        else:
            self.ia_number += 1
            self.child_pids.append(pid)
            print(f"Forked AI process with PID {pid} for {name} on {machine}:{port}")
            return pid

    def clean_up(self):
        for pid in self.child_pids:
            try:
                os.waitpid(pid, 0)
            except ChildProcessError:
                pass
        self.child_pids.clear()
        print("Cleaned up child processes.")


# def main(port, name, machine):
#     ai_generator = AIGenerator(port, name, machine)
#     print(f"Starting AI Generator for {name} on {machine}:{port}")
#     print("Press Ctrl+C to stop")
#     try:
#         while True:
#             pid = ai_generator.generate_ai()
#             if pid is None:
#                 time.sleep(5)
#             else:
#                 time.sleep(1)
#     except KeyboardInterrupt:
#         print("\nAI Generator shutting down...")

# if __name__ == "__main__":
#     import sys
#     if len(sys.argv) != 4:
#         print("Usage: python3 generator.py <port> <name> <machine>")
#         sys.exit(1)
#     port = int(sys.argv[1])
#     name = sys.argv[2]
#     machine = sys.argv[3]
#     main(port, name, machine)