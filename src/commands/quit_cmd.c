/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** quit_cmd
*/

#include "myftp.h"

void quit_cmd(client_t **client)
{
    char *home = NULL;

    home = strdup((*client)->home);
    dprintf((*client)->fd, "221 Goodbye.\n");
    close_client(*client);
    init_client(client, home);
    free(home);
}
