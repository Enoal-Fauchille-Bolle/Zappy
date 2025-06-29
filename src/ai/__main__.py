##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## __main__
##

import sys
import argparse
import os
import time

# Add the parent directory to the path to allow relative imports
if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from ai import ProcessManager
else:
    from .generator import ProcessManager

def validate_port(value: str) -> int:
    """Validate that the port number is within valid range (1024-65535)"""
    try:
        port = int(value)
        if port < 1024 or port > 65535:
            raise argparse.ArgumentTypeError(
                f"Port number must be between 1024 and 65535, got {port}"
            )
        return port
    except ValueError:
        raise argparse.ArgumentTypeError(
            f"Port number must be an integer, got '{value}'"
        )

def parse_argument() -> argparse.Namespace:
    """Parse command line arguments according to Zappy specification"""
    parser = argparse.ArgumentParser(description="Zappy AI client", add_help=False)
    parser.add_argument("-p", "--port", type=validate_port, required=True,
                        help="Port number (1024-65535)")
    parser.add_argument("-n", "--name", type=str, required=True,
                        help="Name of the AI team")
    parser.add_argument("-h", "--hostname", type=str, default="localhost",
                        help="Name of the server machine (localhost by default)")
    parser.add_argument("-help", "--help", action="help", default=argparse.SUPPRESS,
                        help="Show this help message and exit")
    return parser.parse_args()

def main() -> int:
    process_manager = None

    try:
        args = parse_argument()
        process_manager = ProcessManager()

        process_manager.spawn_ai_process(
            args.hostname, args.port, args.name
        )

        print("AI process started. Press Ctrl+C to shutdown...")
        while True:
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\nReceived KeyboardInterrupt, shutting down...")
        if process_manager:
            process_manager.shutdown_all()
        return 0
    except Exception as e:
        print(f"Error: {e}")
        if process_manager:
            process_manager.shutdown_all()
        return 84

if __name__ == "__main__":
    exit_code: int = main()
    sys.exit(exit_code)
