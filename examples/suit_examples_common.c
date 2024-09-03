/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/stat.h>
#include <sys/types.h> // mkdir
#include <unistd.h>
#include <fcntl.h>
#include "suit_examples_common.h"

ssize_t read_from_file(const char *file_path,
                      uint8_t *buf,
                      const size_t buf_len)
{
    ssize_t read_len = 0;
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    read_len = read(fd, buf, buf_len);
    int res = close(fd);
    if (read_len < 0 || res < 0) {
        return 0;
    }
    return read_len;
}

#ifdef _WIN32
const char sep = '\\';
#else
const char sep = '/';
#endif
ssize_t write_to_file(const char *file_path,
                      const void *buf,
                      const size_t buf_len)
{
    ssize_t write_len = 0;
    char dir_name[SUIT_MAX_NAME_LENGTH];
    char *next_sep;
    next_sep = (char *)file_path - 1;

    while ((next_sep = strchr(next_sep + 1, sep)) != NULL) {
        const int dir_name_len = next_sep - file_path;
        memcpy(dir_name, file_path, dir_name_len);
        dir_name[dir_name_len] = '\0';
        mkdir(dir_name, 0775);
    }

    int fd = creat(file_path, 0777);
    write_len = write(fd, buf, buf_len);
    close(fd);
    return write_len;
}
