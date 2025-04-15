/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** data_transfer
*/

#include "myftp.h"

int get_data_client_fd(client_t *client)
{
    int data_client_fd = 0;
    socklen_t addr_len = sizeof(client->data_server_add);

    if (client->dt_mode == PASSIVE) {
        data_client_fd = accept(client->data_server_fd,
        (struct sockaddr *) client->data_server_add,
        &addr_len);
    } else {
        data_client_fd = client->data_server_fd;
    }
    return data_client_fd;
}

void close_data_connection(client_t **client)
{
    close((*client)->data_server_fd);
    free((*client)->data_server_add);
    (*client)->dt_mode = NOTHING;
    (*client)->data_server_fd = -1;
    (*client)->data_server_add = NULL;
}
