/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** stor_cmd
*/

#include "myftp.h"

static void write_str_from_data(client_t **client, int data_client_fd,
int file_fd)
{
    char buff[1024];
    int buff_len = 0;

    dprintf((*client)->fd,
    "150 File status okay; about to open data connection.\n");
    while (1) {
        buff_len = read(data_client_fd, buff, 1023);
        if (buff_len <= 0) {
            break;
        }
        buff[buff_len] = '\0';
        dprintf(file_fd, "%s", buff);
    }
    dprintf((*client)->fd, "226 Closing data connection.\n");
    close_data_connection(client);
}

void stor_cmd(client_t **client, const char *arg)
{
    FILE *file = NULL;
    int data_client_fd = 0;

    file = fopen(arg, "w");
    if (!file) {
        send_msg(client, "550 Couln't create / open file.\n");
        return;
    }
    if (fork() == 0) {
        data_client_fd = get_data_client_fd(*client);
        write_str_from_data(client, data_client_fd, fileno(file));
        close(data_client_fd);
        exit(0);
    }
    close_data_connection(client);
}
