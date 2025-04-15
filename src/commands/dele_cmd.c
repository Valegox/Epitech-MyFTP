/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** dele_cmd
*/

#include "myftp.h"

void dele_cmd(client_t **client, const char *arg)
{
    if (remove(arg) != 0) {
        send_msg(client, "550 Failed to delete file.\n");
    } else {
        send_msg(client, "250 File successfully deleted.\n");
    }
}
