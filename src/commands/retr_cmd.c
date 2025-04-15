/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** retr_cmd
*/

#include <sys/types.h>
#include <sys/stat.h>
#include "myftp.h"

static void send_file_to_data(client_t **client, int data_client_fd,
const char *path)
{
    FILE *file = NULL;
    char buff[1024] = { 0 };

    file = fopen(path, "r");
    if (!file) {
        dprintf((*client)->fd, "550 File not found.\n");
        return;
    }
    dprintf((*client)->fd,
    "150 File status okay; about to open data connection.\n");
    while (fgets(buff, 1024, file)) {
        dprintf(data_client_fd, "%s", buff);
    }
    dprintf((*client)->fd, "226 Closing data connection.\n");
    fclose(file);
    close_data_connection(client);
}

void retr_cmd(client_t **client, const char *arg)
{
    int data_client_fd = 0;

    if (get_file_type(arg) != REGULAR) {
        send_msg(client, "550 File not found.\n");
        return;
    }
    if (fork() == 0) {
        data_client_fd = get_data_client_fd(*client);
        send_file_to_data(client, data_client_fd, arg);
        close(data_client_fd);
        exit(0);
    }
    close_data_connection(client);
}
