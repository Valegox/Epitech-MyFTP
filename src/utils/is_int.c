/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** utils: check if string is an int
*/

#include <ctype.h>

int is_int(const char *str)
{
    if (*str == '-')
        ++str;
    if (!*str)
        return 0;
    while (*str) {
        if (!isdigit(*str))
            return 0;
        else
            str++;
    }
    return 1;
}
