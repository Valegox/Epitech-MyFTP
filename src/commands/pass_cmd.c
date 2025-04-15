/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** pass_cmd
*/

#include "myftp.h"

static void clean_auth_state(client_t **client)
{
    (*client)->auth_state = NOT_LOGGED_IN;
    if ((*client)->username != NULL) {
        free((*client)->username);
        (*client)->username = NULL;
    }
}

static int check_input(client_t **client, const char *password)
{
    if (strcasecmp((*client)->username, "Anonymous") != 0) {
        send_msg(client, "530 This username does not exist.\n");
        clean_auth_state(client);
        return 0;
    }
    if (strlen(password) != 0) {
        send_msg(client, "530 Wrong password.\n");
        clean_auth_state(client);
        return 0;
    }
    return 1;
}

void pass_cmd(client_t **client, const char *password)
{
    switch ((*client)->auth_state) {
        case NOT_LOGGED_IN:
            send_msg(client, "503 Login with USER first.\n");
            break;
        case USER_SENT:
            if (!check_input(client, password)) {
                return;
            }
            send_msg(client, "230 User logged in, proceed.\n");
            (*client)->auth_state = LOGGED_IN;
            break;
        case LOGGED_IN:
            send_msg(client, "230 Already logged in.\n");
            break;
        default:
            break;
    }
}
