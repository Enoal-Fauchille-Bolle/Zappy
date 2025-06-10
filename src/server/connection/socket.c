/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Socket Management
*/

#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "options_parser/options.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Sets up a socket to listen for incoming connections
 *
 * This function configures the specified socket file descriptor to listen
 * for incoming client connections with a maximum queue size of MAX_CLIENTS.
 *
 * @param server_sockfd The socket file descriptor to set up for listening
 * @return true (SUCCESS) if the socket is successfully set to listen mode,
 *         false (FAILURE) if the listen operation fails
 */
static bool listen_socket(int server_sockfd)
{
    if (listen(server_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Binds a socket to a server address
 *
 * This function binds the given server socket file descriptor to the specified
 * server address structure. If the binding fails, an error message is printed
 * to stderr using perror.
 *
 * @param server_sockfd The server socket file descriptor to bind
 * @param server_addr Pointer to the server address structure to bind to
 * @return true on success (SUCCESS), false on failure (FAILURE)
 */
static bool bind_socket(int server_sockfd, struct sockaddr_in *server_addr)
{
    socklen_t addr_len = sizeof(*server_addr);

    if (bind(server_sockfd, (struct sockaddr *)server_addr, addr_len) == -1) {
        perror("bind");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Initialize a sockaddr_in structure for server socket binding
 *
 * Creates and configures a sockaddr_in structure with IPv4 family,
 * specified port, and loopback address (127.0.0.1).
 *
 * @param port The port number to bind the socket to
 * @return struct sockaddr_in Configured socket address structure
 */
static struct sockaddr_in init_sockin(int port)
{
    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return server_addr;
}

/**
 * @brief Creates a TCP socket file descriptor
 *
 * Creates a new socket using the IPv4 protocol family and TCP stream protocol.
 * Prints an error message to stderr if socket creation fails.
 *
 * @return int The socket file descriptor on success, -1 on failure
 */
static int setup_socket_fd(void)
{
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sockfd == -1) {
        perror("socket");
    }
    return server_sockfd;
}

/**
 * @brief Sets up a server socket for incoming connections
 *
 * Initializes a server socket by creating a file descriptor, binding it to the
 * specified port, and setting it to listen for incoming connections.
 *
 * @param server Pointer to the server structure to initialize
 * @param port Port number to bind the socket to
 * @return true if socket setup is successful, false otherwise
 *
 * @note On failure, the function automatically cleans up allocated resources
 *       including closing the socket file descriptor and freeing the server
 *       structure
 */
bool setup_socket(server_t *server, int port)
{
    server->port = port;
    server->sockfd = setup_socket_fd();
    if (server->sockfd == -1) {
        free(server);
        return FAILURE;
    }
    server->addr = init_sockin(port);
    if (bind_socket(server->sockfd, &(server->addr)) == FAILURE ||
        listen_socket(server->sockfd) == FAILURE) {
        close(server->sockfd);
        free(server);
        return FAILURE;
    }
    return SUCCESS;
}
