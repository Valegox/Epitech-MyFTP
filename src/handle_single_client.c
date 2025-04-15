/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** handle_single_client
*/

#include <errno.h>
#include "myftp.h"

void send_msg(client_t **client, char *msg)
{
    (*client)->message = concat_str((*client)->message, msg);
}

static void bufferize_cmd(client_t **client, char *cmd,
const unsigned int size)
{
    char *clean_cmd = NULL;

    clean_cmd = strdup(cmd);
    if (clean_cmd[size - 1] == '\n')
        clean_cmd[size - 1] = '\0';
    if (clean_cmd[size - 2] == '\r')
        clean_cmd[size - 2] = '\0';
    if ((*client)->buffered_cmd == NULL) {
        (*client)->buffered_cmd = strdup(clean_cmd);
    } else {
        (*client)->buffered_cmd = concat_str((*client)->buffered_cmd,
        clean_cmd);
    }
    free(clean_cmd);
}

void listen_to_client(client_t **client)
{
    char buffer[1024];
    int size = 0;

    size = read((*client)->fd, buffer, 1024);
    if (size > 0) {
        buffer[size] = '\0';
        bufferize_cmd(client, buffer, size);
        if (buffer[size - 1] != '\n')
            return;
        chdir((*client)->current_path);
        handle_commands(client);
        free((*client)->buffered_cmd);
        (*client)->buffered_cmd = NULL;
    } else if (size == 0) {
        quit_cmd(client);
    }
}

void welcome_client(struct sockaddr *addr, int server_fd, client_t **clients)
{
    socklen_t addrlen = sizeof(*addr);
    int client_fd = 0;

    client_fd = accept(server_fd, addr, &addrlen);
    if (client_fd < 0) {
        fprintf(stderr, "Accept failed [%s]\n", strerror(errno));
    }
    printf("Accepted a new connection with fd: %d\n", client_fd);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (clients[i]->fd < 0) {
            clients[i]->fd = client_fd;
            send_msg(&clients[i], "220 Service ready for new user.\n");
            break;
        }
    }
}
