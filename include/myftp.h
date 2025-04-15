/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** header
*/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAX_CONNECTIONS 10

typedef enum auth_state_e {
    NOT_LOGGED_IN = 0,
    USER_SENT,
    LOGGED_IN
} auth_state_t;

typedef enum data_transfer_mode_e {
    NOTHING = 0,
    ACTIVE,
    PASSIVE
} data_transfer_mode_t;

typedef enum file_type_e {
    UNKNOWN = 0,
    REGULAR,
    DIRECTORY
} file_type_t;

typedef struct client_s {
    int fd;
    data_transfer_mode_t dt_mode;
    int data_server_fd;
    struct sockaddr_in *data_server_add;
    auth_state_t auth_state;
    char *username;
    char *home;
    char *current_path;
    char *message;
    char *buffered_cmd;
} client_t;

char *replace_char(char *str, const char find, const char replace);
void remove_char(char *str, const char find);
int is_int(const char *str);
file_type_t get_file_type(const char *path);
char *concat_str(char *str1, const char *str2);
int launch_server(unsigned int port, const char *path);
struct sockaddr_in get_sockaddr(in_addr_t address, unsigned int port);
void init_client(client_t **client, const char *path);
void close_client(client_t *client);
void send_msg(client_t **client, char *msg);
void handle_client_connection(struct sockaddr_in *addr, int server_fd,
const char *path);
void welcome_client(struct sockaddr *addr, int server_fd, client_t **clients);
void listen_to_client(client_t **client);
int get_data_client_fd(client_t *client);
void close_data_connection(client_t **client);
void handle_commands(client_t **client);
void user_cmd(client_t **client, const char *username);
void pass_cmd(client_t **client, const char *password);
void quit_cmd(client_t **client);
void pwd_cmd(client_t **client);
void cwd_cmd(client_t **client, const char *arg);
void cdup_cmd(client_t **client);
void dele_cmd(client_t **client, const char *arg);
void pasv_cmd(client_t **client);
void port_cmd(client_t **client, const char *arg);
void retr_cmd(client_t **client, const char *arg);
void list_no_arg_cmd(client_t **client);
void list_arg_cmd(client_t **client, const char *arg);
void stor_cmd(client_t **client, const char *arg);
void help_with_arg_cmd(client_t **client, const char *arg);
void help_no_arg_cmd(client_t **client);
void noop_cmd(client_t **client);
