/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** help_cmd
*/

#include "myftp.h"

static void write_help(client_t **client)
{
    send_msg(client, "214-The following commands are recognized.\n");
    send_msg(client,
    " CDUP CWD DELE HELP LIST NOOP PASS PASV PORT PWD QUIT RETR STOR USER\n");
    send_msg(client, "214 Help OK.\n");
}

void help_with_arg_cmd(client_t **client,
__attribute__((unused)) const char *arg)
{
    write_help(client);
}

void help_no_arg_cmd(client_t **client)
{
    write_help(client);
}
