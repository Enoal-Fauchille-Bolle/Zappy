##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## __main__
##

import sys
import argparse
import asyncio
import os

# Add the parent directory to the path to allow relative imports
if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from refactored.client import ZappyAi
else:
    from .client import ZappyAi

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

async def main(argc: int, argv: list[str]) -> int:
    try:
        args = parse_argument()

        ai = ZappyAi(args.hostname, args.port, args.name)
        await ai.run()

        return 0
    except KeyboardInterrupt:
        return 0
    except Exception as e:
        print(f"Error: {e}")
        return 84

if __name__ == "__main__":
    try:
        exit_code: int = asyncio.run(main(len(sys.argv), sys.argv))
    except KeyboardInterrupt:
        print("\nAi shutting down")
        sys.exit(0)
    sys.exit(exit_code)
