/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** welcome_client
*/

#include "myftp.h"

static int handle_cmd_no_arg(char *buffer, client_t **client, char *cmd,
void (*func)(client_t **))
{
    int cmd_len = strlen(cmd);

    if (strncmp(buffer, cmd, cmd_len) == 0 && (buffer[cmd_len] == ' ' ||
    buffer[cmd_len] == '\r' || buffer[cmd_len] == '\0')) {
        func(client);
        return 1;
    }
    return 0;
}

static int handle_cmd_with_arg(char *buffer, client_t **client, char *cmd,
void (*func)(client_t **, const char *))
{
    int cmd_len = strlen(cmd);

    if (strncmp(buffer, cmd, cmd_len) == 0) {
        if (buffer[cmd_len] == ' ') {
            func(client, buffer + cmd_len + 1);
            return 1;
        }
        if (strncmp(cmd, "LIST", 4) == 0) {
            return 0;
        }
        if (buffer[cmd_len] == '\r' || buffer[cmd_len] == '\0') {
            send_msg(client, "501 Syntax error in parameters or arguments.\n");
        } else {
            send_msg(client, "500 Unknown command.\n");
        }
        return 1;
    }
    return 0;
}

static int handle_data_transfer_commands(char *cmd, client_t **client)
{
    int list_arg = 0;

    if (strncmp(cmd, "LIST", 4) == 0 || strncmp(cmd, "RETR ", 5) == 0 ||
    strncmp(cmd, "STOR ", 5) == 0) {
        if ((*client)->data_server_fd == -1) {
            send_msg(client, "425 Use PORT or PASV first.\n");
            return 1;
        }
        handle_cmd_with_arg(cmd, client, "RETR", retr_cmd);
        list_arg = handle_cmd_with_arg(cmd, client, "LIST", list_arg_cmd);
        if (!list_arg) {
            handle_cmd_no_arg(cmd, client, "LIST", list_no_arg_cmd);
        }
        handle_cmd_with_arg(cmd, client, "STOR", stor_cmd);
        return 1;
    }
    return 0;
}

static void handle_user_commands(char *cmd, client_t **client)
{
    int found = 0;
    char *cmds_with_arg[3] = {"CWD", "PORT", "DELE"};
    void (*functions_with_args[3])(client_t **, const char *) = {cwd_cmd,
    port_cmd, dele_cmd};
    char *cmds_no_arg[5] = {"CDUP", "PWD", "PASV", "HELP", "NOOP"};
    void (*functions_no_args[5])(client_t **) = {cdup_cmd, pwd_cmd, pasv_cmd,
    help_no_arg_cmd, noop_cmd};

    for (int i = 0; i < 3; i++) {
        found += handle_cmd_with_arg(cmd, client, cmds_with_arg[i],
        functions_with_args[i]);
    }
    for (int i = 0; i < 5; i++) {
        found += handle_cmd_no_arg(cmd, client, cmds_no_arg[i],
        functions_no_args[i]);
    }
    found += handle_data_transfer_commands(cmd, client);
    if (!found) {
        send_msg(client, "500 Unknown command.\n");
    }
}

void handle_commands(client_t **client)
{
    int found = 0;
    char *cmd = NULL;

    cmd = (*client)->buffered_cmd;
    found += handle_cmd_with_arg(cmd, client, "USER", user_cmd);
    found += handle_cmd_with_arg(cmd, client, "PASS", pass_cmd);
    found += handle_cmd_no_arg(cmd, client, "QUIT", quit_cmd);
    if (!found && (*client)->auth_state != LOGGED_IN) {
        send_msg(client, "530 Please login with USER and PASS.\n");
        return;
    }
    if (!found) {
        handle_user_commands(cmd, client);
    }
}
