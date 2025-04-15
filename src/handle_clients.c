/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** handle_clients
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include "myftp.h"

static fd_set get_fd_set(client_t **clients)
{
    fd_set read_fd_set;

    FD_ZERO(&read_fd_set);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (clients[i]->fd >= 0) {
            FD_SET(clients[i]->fd, &read_fd_set);
        }
    }
    return read_fd_set;
}

static void handle_client_input(struct sockaddr_in *addr, int server_fd,
client_t **clients)
{
    fd_set read_fd_set;
    fd_set write_fd_set;

    read_fd_set = get_fd_set(clients);
    FD_SET(server_fd, &read_fd_set);
    write_fd_set = get_fd_set(clients);
    select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);
    if (FD_ISSET(server_fd, &read_fd_set)) {
        welcome_client((struct sockaddr *) addr, server_fd, clients);
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (clients[i]->fd > 0 && FD_ISSET(clients[i]->fd, &write_fd_set)) {
            dprintf(clients[i]->fd, "%s", clients[i]->message);
            free(clients[i]->message);
            clients[i]->message = strdup("");
        }
        if (clients[i]->fd > 0 && FD_ISSET(clients[i]->fd, &read_fd_set)) {
            listen_to_client(&clients[i]);
        }
    }
}

static client_t **init_clients(const char *path)
{
    client_t **clients = NULL;
    int i = 0;

    clients = malloc(sizeof(client_t *) * (MAX_CONNECTIONS + 1));
    for (i = 0; i < MAX_CONNECTIONS; i++) {
        clients[i] = malloc(sizeof(client_t));
        init_client(&clients[i], path);
    }
    clients[i] = NULL;
    return clients;
}

static void close_clients(client_t **clients)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        close_client(clients[i]);
        free(clients[i]);
    }
    free(clients);
}

void handle_client_connection(struct sockaddr_in *addr, int server_fd,
const char *path)
{
    client_t **clients = NULL;

    clients = init_clients(path);
    while (1) {
        handle_client_input(addr, server_fd, clients);
    }
    close_clients(clients);
}
