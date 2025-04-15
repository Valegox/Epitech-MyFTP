/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** utils: replace char
*/

#include <string.h>

char *replace_char(char *str, const char find, const char replace)
{
    char *current_pos = NULL;

    current_pos = strchr(str, find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}
