/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** user_cmd
*/

#include "myftp.h"

void user_cmd(client_t **client, const char *username)
{
    if ((*client)->auth_state == LOGGED_IN) {
        send_msg(client, "230 Already logged in.\n");
        return;
    }
    send_msg(client, "331 User name okay, need password.\n");
    (*client)->auth_state = USER_SENT;
    (*client)->username = strdup(username);
}
