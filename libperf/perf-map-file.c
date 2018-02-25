/*
 *   libperfmap: a JVM agent to create perf-<pid>.map files for consumption
 *               with linux perf-tools
 *   Copyright (C) 2013-2015 Johannes Rudolph<johannes.rudolph@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "perf-map-file.hpp"

#include <sys/types.h>
#include <stdio.h>

#ifdef __APPLE__
#include <stdlib.h>
#else
#include <error.h>
#endif

#include <errno.h>
#include <unistd.h>

FILE *perf_map_open(void) {
    char filename[500];
    pid_t my_pid = getpid();
    FILE* res = NULL;

    snprintf(filename, sizeof(filename), "/tmp/perf-%d.map", my_pid);
    res = fopen(filename, "w");
    if (!res) {
#ifdef __APPLE__
        fprintf(stderr, "Couldn't open %s: errno(%d)", filename, errno);
        exit(0);
#else
        error(0, errno, "Couldn't open %s.", filename);
#endif
    }
    return res;
}

int perf_map_close(FILE *fp) {
    if (fp)
        return fclose(fp);
    else
        return 0;
}

void perf_map_write_entry(FILE *method_file, const void* code_addr, unsigned int code_size, const char* entry) {
    if (method_file) {
        fprintf(method_file, "%lx %x %s\n", (unsigned long) code_addr, code_size, entry);
	fflush(method_file);
    } else {
	printf("Can't write content to perf file!");
    }
}
