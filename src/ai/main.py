#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main file for ai
##

import sys
from parser import check_good_args
from generator import AIGenerator

def main(argc, argv):
    port, name, machine = None, None, "localhost"
    if argc == 1 or argc == 2 and argv[1] == "--help":
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        return 0
    valid_args, port, name, machine = check_good_args(argv, port, name, machine)
    if not valid_args:
        return 84
    generator = AIGenerator(port, name, machine)
    generator.generate_ai()
    return 0

if __name__ == "__main__":
    exit_code = main(len(sys.argv), sys.argv)
    sys.exit(exit_code)