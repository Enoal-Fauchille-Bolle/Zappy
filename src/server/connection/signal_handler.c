/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Signal Handler
*/

#include "connection/signal_handler.h"
#include "debug_categories.h"
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>

/**
 * @brief Sets up signal handling for the server
 *
 * Blocks SIGINT signal and creates a signalfd to handle it synchronously
 * in the main event loop.
 *
 * @param server Pointer to the server structure
 * @return int Signal file descriptor on success, -1 on failure
 */
int setup_signal_handler(void)
{
    sigset_t mask;
    int sfd;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        return -1;
    }
    sfd = signalfd(-1, &mask, SFD_CLOEXEC);
    if (sfd == -1) {
        perror("signalfd");
        return -1;
    }
    return sfd;
}

/**
 * @brief Handles signals received through signalfd
 *
 * Reads signal information from the signalfd and processes it accordingly.
 * Currently handles SIGINT for graceful shutdown.
 *
 * @param server Pointer to the server structure
 * @param signal_fd Signal file descriptor to read from
 * @return bool true to continue running, false to shutdown
 */
bool handle_signal(int signal_fd, bool debug)
{
    struct signalfd_siginfo si;
    ssize_t s = read(signal_fd, &si, sizeof(si));

    if (s == -1 || s != sizeof(si)) {
        return true;
    }
    if (si.ssi_signo == SIGINT) {
        printf("\n");
        debug_server(debug, "Received SIGINT, shutting down gracefully");
        return false;
    }
    return true;
}
