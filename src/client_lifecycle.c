/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** client_lifecycle
*/

#include "myftp.h"

void init_client(client_t **client, const char *path)
{
    (*client)->fd = -1;
    (*client)->dt_mode = NOTHING;
    (*client)->data_server_fd = -1;
    (*client)->data_server_add = NULL;
    (*client)->auth_state = NOT_LOGGED_IN;
    (*client)->username = NULL;
    (*client)->home = strdup(path);
    (*client)->current_path = strdup(path);
    (*client)->message = strdup("");
    (*client)->buffered_cmd = NULL;
}

void close_client(client_t *client)
{
    if (client->fd != -1)
        close(client->fd);
    if (client->data_server_fd != -1)
        close(client->data_server_fd);
    if (client->data_server_add != NULL)
        free(client->data_server_add);
    if (client->username != NULL)
        free(client->username);
    if (client->home != NULL)
        free(client->home);
    if (client->current_path != NULL)
        free(client->current_path);
    if (client->message != NULL)
        free(client->message);
    if (client->buffered_cmd != NULL)
        free(client->buffered_cmd);
}
