/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** cdup_cmd
*/

#include "myftp.h"

void cdup_cmd(client_t **client)
{
    cwd_cmd(client, "..");
}
