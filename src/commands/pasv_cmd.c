/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** pasv_cmd
*/

#include "myftp.h"

static void write_address_and_port(client_t **client, struct in_addr addr,
in_port_t port)
{
    int intPort = ntohs(port);
    int first = intPort / 256;
    int second = intPort % 256;
    char firstStr[5] = { 0 };
    char secondStr[5] = { 0 };

    sprintf(firstStr, "%i", first);
    sprintf(secondStr, "%i", second);
    send_msg(client, "227 Entering Passive Mode (");
    send_msg(client, replace_char(inet_ntoa(addr), '.', ','));
    send_msg(client, ",");
    send_msg(client, firstStr);
    send_msg(client, ",");
    send_msg(client, secondStr);
    send_msg(client, ")\n");
}

void pasv_cmd(client_t **client)
{
    int data_server_fd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in my_addr = get_sockaddr(inet_addr("127.0.0.1"), 0);
    socklen_t addr_len = sizeof(struct sockaddr_in);

    setsockopt(data_server_fd, SOL_SOCKET, SO_REUSEADDR,
    &(int){ 1 }, sizeof(int));
    bind(data_server_fd, (const struct sockaddr *) &my_addr,
        sizeof(struct sockaddr_in));
    listen(data_server_fd, 10);
    getsockname(data_server_fd, (struct sockaddr *) &my_addr, &addr_len);
    write_address_and_port(client, my_addr.sin_addr, my_addr.sin_port);
    (*client)->dt_mode = PASSIVE;
    (*client)->data_server_fd = data_server_fd;
    (*client)->data_server_add = malloc(sizeof(struct sockaddr_in));
    memcpy((*client)->data_server_add, &my_addr, sizeof(struct sockaddr_in));
}
