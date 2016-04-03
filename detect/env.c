/*
 * Detect GDB using env
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int gdb_columns() {
    return (getenv("COLUMNS") != NULL);
}

int gdb_lines() {
    return (getenv("LINES") != NULL);
}

int gdb_path() {
    char *gdb_path = getenv("_");
    char *block_path = strtok(gdb_path, "/");
    char *str = NULL;
    while (block_path != NULL) {
        str = block_path;
        block_path = strtok(NULL, "/");
    }
    if(!strcmp(str,"gdb")) {
        return 1;
    } else {
        return 0;
    };
}

