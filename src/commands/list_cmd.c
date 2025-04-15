/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** list_cmd
*/

#include "myftp.h"

static FILE *simulate_ls(client_t **client, const char *path)
{
    FILE *fp = NULL;
    char *command = NULL;

    command = malloc(sizeof(char) * (strlen(path) + 7));
    memcpy(command, "ls -l ", 6);
    memcpy(command + 6, path, strlen(path));
    command[strlen(path) + 6] = '\0';
    fp = popen(command, "r");
    free(command);
    if (fp == NULL) {
        dprintf((*client)->fd, "550 Failed to run command\n");
        exit(1);
    }
    return fp;
}

static void send_ls_to_data(client_t **client, int data_client_fd,
const char *path, const file_type_t type)
{
    FILE *fp = NULL;
    char *buff = NULL;
    size_t buff_len = 1024;
    int i = 0;

    fp = simulate_ls(client, path);
    if (!fp)
        return;
    dprintf((*client)->fd, "150 Here comes the directory listing.\n");
    while (getline(&buff, &buff_len, fp) != -1) {
        if (i > 0 || type == REGULAR)
            dprintf(data_client_fd, "%s", buff);
        free(buff);
        buff = NULL;
        i++;
    }
    free(buff);
    pclose(fp);
    dprintf((*client)->fd, "226 Directory send OK.\n");
    close_data_connection(client);
}

static void list_cmd(client_t **client, const char *path,
const file_type_t type)
{
    int data_client_fd = 0;

    if (fork() == 0) {
        data_client_fd = get_data_client_fd(*client);
        send_ls_to_data(client, data_client_fd, path, type);
        close(data_client_fd);
        exit(0);
    }
    close_data_connection(client);
}

void list_arg_cmd(client_t **client, const char *arg)
{
    file_type_t file_type = UNKNOWN;

    file_type = get_file_type(arg);
    if (file_type == UNKNOWN) {
        send_msg(client, "550 Couldn't find file or directory.\n");
        return;
    }
    list_cmd(client, arg, file_type);
}

void list_no_arg_cmd(client_t **client)
{

    list_cmd(client, (*client)->current_path, DIRECTORY);
}
