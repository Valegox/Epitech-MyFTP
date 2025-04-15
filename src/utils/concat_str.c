/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** utils: concat
*/

#include <stdlib.h>
#include "myftp.h"

char *concat_str(char *str1, const char *str2)
{
    char *result = NULL;

    result = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(result, str1);
    strcat(result, str2);
    free(str1);
    return result;
}
