/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** server
*/

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include "myftp.h"

struct sockaddr_in get_sockaddr(in_addr_t address, unsigned int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = address;
    memset(&(addr.sin_zero), '\0', 8);
    return addr;
}

int launch_server(unsigned int port, const char *path)
{
    int server_fd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in my_addr = get_sockaddr(inet_addr("127.0.0.1"), port);

    if (server_fd < 0)
        return 84;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    if (bind(server_fd, (const struct sockaddr *) &my_addr,
    sizeof(struct sockaddr_in)) < 0)
        return 84;
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
        return 84;
    printf("Listening on port %i...\n", port);
    handle_client_connection(&my_addr, server_fd, path);
    close(server_fd);
    return 0;
}
