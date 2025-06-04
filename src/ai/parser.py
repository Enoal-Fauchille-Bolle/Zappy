##
## EPITECH PROJECT, 2025
## zappy
## File description:
## parser for ai
##

"""@brief This module provides functions to parse command line arguments for the Zappy AI client.
It validates the port, name, and machine arguments, ensuring they meet the required criteria.
"""
def validate_port(port_str, flags):
    if flags["p"]:
        print("ERROR: Port already set.")
        return False, None
    flags["p"] = True
    try:
        port = int(port_str)
        if not (1024 <= port <= 65535):
            print("ERROR: Port must be between 1024 and 65535.")
            return False, None
        return True, port
    except ValueError:
        print("ERROR: Port must be a number.")
        return False, None

"""@brief Validates the name and machine arguments.
Checks if the name is between 3 and 20 characters long, and if the machine is a valid IP address or 'localhost'.
Returns a tuple indicating success and the validated value.
"""
def validate_name_and_machine(arg_type, value, flags):
    key = "n" if arg_type == "-n" else "h"
    error_msg = "Name" if arg_type == "-n" else "Machine"
    if flags[key]:
        print(f"ERROR: {error_msg} already set.")
        return False, None
    flags[key] = True
    if arg_type == "-n":
        if len(value) < 3 or len(value) > 20:
            print("ERROR: Name must be between 3 and 20 characters long.")
            return False, None
        return True, value
    else:
        if value == "localhost":
            return True, value
        parts = value.split('.')
        if len(parts) != 4:
            print("ERROR: IP address must have exactly 4 parts (e.g., 192.168.1.1).")
            return False, None
        try:
            for part in parts:
                if not part.isdigit() or not (0 <= int(part) <= 255):
                    print("ERROR: IP address parts must be between 0 and 255.")
                    return False, None
            return True, value
        except ValueError:
            print("ERROR: Invalid IP address format.")
            return False, None

"""@brief Checks the validity of command line arguments for the Zappy AI client.
Validates the port, name, and machine arguments based on the provided flags.
Returns a tuple indicating success and the validated port, name, and machine.
"""
def check_value(args, port, name, machine, flags):
    flag, value = args[0], args[1]
    if flag == "-p":
        success, valid_port = validate_port(value, flags)
        if not success:
            return False, port, name, machine
        port = valid_port
    elif flag in ["-n", "-h"]:
        success, valid_value = validate_name_and_machine(flag, value, flags)
        if not success:
            return False, port, name, machine
        if flag == "-n":
            name = valid_value
        else:
            machine = valid_value
    return True, port, name, machine

"""@brief Checks the command line arguments for the Zappy AI client.
Validates the number of arguments and checks each argument for correctness.
Returns a tuple indicating success and the validated port, name, and machine.
"""
def check_good_args(args, port, name, machine):
    if len(args) != 7 and len(args) != 5:
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        return False, port, name, machine
    flags = {"p": False, "n": False, "h": False}
    for i in range(1, len(args), 2):
        if i + 1 >= len(args):
            return False, port, name, machine
        if args[i] not in ["-p", "-n", "-h"]:
            return False, port, name, machine
        success, port, name, machine = check_value([args[i], args[i+1]], port, name, machine, flags)
        if not success:
            return False, port, name, machine
    return True, port, name, machine
