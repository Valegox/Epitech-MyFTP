/*
** EPITECH PROJECT, 2023
** MyFtp
** File description:
** utils: get file type
*/

#include <sys/types.h>
#include <sys/stat.h>
#include "myftp.h"

file_type_t get_file_type(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0) {
        return UNKNOWN;
    }
    if (S_ISDIR(path_stat.st_mode)) {
        return DIRECTORY;
    }
    return REGULAR;
}
