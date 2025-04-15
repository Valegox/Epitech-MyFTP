/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** utils: remove char
*/

#include <string.h>

static void shift_string(char *str, int *i, int *len)
{
    for (int j = (*i); j < (*len); j++) {
        str[j] = str[j + 1];
    }
    (*len)--;
    (*i)--;
}

void remove_char(char *str, const char find)
{
    int len = 0;

    len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == find) {
            shift_string(str, &i, &len);
        }
    }
}
