#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main file for zappy
##

import sys

def check_value(args, port, name, machine, flags):
    if args[0] == "-p":
        if flags["p"]:
            print("ERROR: Port already set.")
            return False, port, name, machine
        flags["p"] = True
        try:
            if not (1024 <= int(args[1]) <= 65535):
                print("ERROR: Port must be between 1024 and 65535.")
                return False, port, name, machine
            port = int(args[1])
        except ValueError:
            print("ERROR: Port must be a number.")
            return False, port, name, machine
    elif args[0] == "-n":
        if flags["n"]:
            print("ERROR: Name already set.")
            return False, port, name, machine
        flags["n"] = True
        if len(args[1]) < 3 or len(args[1]) > 20:
            print("ERROR: Name must be between 3 and 20 characters long.")
            return False, port, name, machine
        name = args[1]
    elif args[0] == "-h":
        if flags["h"]:
            print("ERROR: Machine already set.")
            return False, port, name, machine
        flags["h"] = True
        parts = args[1].split('.')
        if len(parts) != 4:
            print("ERROR: IP address must have exactly 4 parts (e.g., 192.168.1.1).")
            return False, port, name, machine
        try:
            for part in parts:
                if not part.isdigit() or not (0 <= int(part) <= 255):
                    print("ERROR: IP address parts must be between 0 and 255.")
                    return False, port, name, machine
        except ValueError:
            print("ERROR: Invalid IP address format.")
            return False, port, name, machine
        machine = args[1]
    return True, port, name, machine

def check_good_args(args, port, name, machine):
    if len(args) != 7 and len(args) != 5:
        return False, port, name, machine
    flags = {"p": False, "n": False, "h": False}
    for i in range(1, len(args), 2):
        print(f"Checking argument {i}: {args[i]}")
        if i + 1 >= len(args):
            return False, port, name, machine
        if args[i] not in ["-p", "-n", "-h"]:
            return False, port, name, machine
        success, port, name, machine = check_value([args[i], args[i+1]], port, name, machine, flags)
        if not success:
            return False, port, name, machine
    return True, port, name, machine

def main(argc, argv):
    port, name, machine = None, None, "localhost"
    if argc == 2 and argv[1] == "--help":
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        return 0
    print(f"Arguments received: {argv}")
    valid_args, port, name, machine = check_good_args(argv, port, name, machine)
    if not valid_args:
        return 84
    print(f"Connecting to server {machine} on port {port} with name {name}")
    return 0

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("ERROR: Not enough arguments.")
        sys.exit(84)
    exit_code = main(len(sys.argv), sys.argv)
    sys.exit(exit_code)