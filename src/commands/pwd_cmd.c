/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** pwd_cmd
*/

#include "myftp.h"

void pwd_cmd(client_t **client)
{
    send_msg(client, "257 \"");
    send_msg(client, (*client)->current_path);
    send_msg(client, "\" is the current directory\n");
}
