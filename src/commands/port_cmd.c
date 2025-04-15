/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** port_cmd
*/

#include "myftp.h"
#include <errno.h>

static char *parse_address(const char *arg, char *tmp)
{
    char *address = NULL;

    address = malloc(sizeof(char) * strlen(arg) + 1);
    address[0] = '\0';
    tmp = strtok((char *) arg, ",");
    for (int i = 0; i < 3; i++) {
        if (!tmp)
            return NULL;
        address = strcat(address, tmp);
        address = strcat(address, ".");
        tmp = strtok(NULL, ",");
    }
    if (!tmp)
        return NULL;
    address = strcat(address, tmp);
    return address;
}

static int are_operands_int(char *first, char *second)
{
    if (!is_int(first) || !is_int(second)) {
        free(first);
        free(second);
        return 0;
    }
    return 1;
}

static int parse_port(char *tmp)
{
    char *first_operand = NULL;
    char *second_operand = NULL;
    int result = 0;

    tmp = strtok(NULL, ",");
    if (!tmp)
        return -1;
    first_operand = strdup(tmp);
    tmp = strtok(NULL, ",");
    if (!tmp)
        return -1;
    second_operand = strdup(tmp);
    if (!are_operands_int(first_operand, second_operand)) {
        return -1;
    }
    result = (atoi(first_operand) * 256) + atoi(second_operand);
    free(first_operand);
    free(second_operand);
    return result;
}

static struct sockaddr_in parse_sockaddr(const char *arg)
{
    char *tmp = NULL;
    char *address = NULL;
    int port = 0;
    struct sockaddr_in addr;

    address = parse_address(arg, tmp);
    port = parse_port(tmp);
    addr = get_sockaddr(inet_addr(address), port);
    free(address);
    return addr;
}

void port_cmd(client_t **client, const char *arg)
{
    int data_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in my_addr = parse_sockaddr(arg);
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int connect_status =
    connect(data_socket, (const struct sockaddr *) &my_addr, addr_len);

    if (connect_status == -1) {
        send_msg(client, "425 Can't open data connection.\n");
        return;
    }
    send_msg(client, "200 PORT command successful.\n");
    (*client)->dt_mode = ACTIVE;
    (*client)->data_server_fd = data_socket;
    (*client)->data_server_add = malloc(sizeof(struct sockaddr_in));
    memcpy((*client)->data_server_add, &my_addr, sizeof(struct sockaddr_in));
}
