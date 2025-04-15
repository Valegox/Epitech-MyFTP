/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** main
*/

#include "myftp.h"

void print_help(void)
{
    printf("USAGE: ./myftp port path\n");
    printf("       port  is the port number on ");
    printf("which the server socket listens\n");
    printf("       path  is the path to the home ");
    printf("directory for the \e[3mAnonymous\e[0m user\n");
}

int handle_errors(int ac, char **av)
{
    if (ac < 3) {
        print_help();
        return 84;
    }
    if (strcmp(av[1], "-help") == 0) {
        print_help();
        return 0;
    }
    if (!is_int(av[1]) || atoi(av[1]) < 0) {
        fprintf(stderr, "Port must be a positive number\n");
        return 84;
    }
    if (chdir(av[2]) != 0) {
        fprintf(stderr, "Wrong path\n");
        return 84;
    }
    return 1;
}

int main(int ac, char **av)
{
    char path[4096] = {0};
    int status = 0;

    status = handle_errors(ac, av);
    if (status != 1)
        return status;
    if (getcwd(path, sizeof(path)) == NULL) {
        fprintf(stderr, "Couldn't get working directory from path\n");
        return 84;
    }
    if (launch_server(atoi(av[1]), path) == 84) {
        fprintf(stderr, "Couldn't launch server\n");
        return 84;
    }
    return 0;
}
