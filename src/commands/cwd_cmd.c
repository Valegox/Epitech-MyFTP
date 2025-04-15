/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** cwd_cmd
*/

#include "myftp.h"

void cwd_cmd(client_t **client, const char *arg)
{
    char path[1024];

    if (chdir(arg) != 0) {
        send_msg(client, "550 Failed to change directory.\n");
        return;
    }
    if (getcwd(path, sizeof(path)) == NULL) {
        send_msg(client, "550 Couldn't get directory.\n");
        return;
    }
    if (strncmp(path, (*client)->home, strlen((*client)->home)) != 0) {
        send_msg(client, "550 You can't go outside the home directory.\n");
        return;
    }
    free((*client)->current_path);
    (*client)->current_path = strdup(path);
    send_msg(client, "250 Directory successfully changed.\n");
}
