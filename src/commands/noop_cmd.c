/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** noop_cmd
*/

#include "myftp.h"

void noop_cmd(client_t **client)
{
    send_msg(client, "200 NOOP ok.\n");
}
