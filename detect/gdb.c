/*
 * Various tricks to detect GDB or any related GNU tools
 *
 */


#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>


#define check_strings(str_buff) (strstr(str_buff, "gdb") || strstr(str_buff, "ltrace") || strstr(str_buff, "strace"))

/*
 * This is an old well known bug (#7912)
 *
 * GDB leaks 2 file descriptors when it opens a program to be debugged.
 * Both file descriptors are pointing to the file being debugged.
 */
int dbg_file_descriptor(){
    FILE* fd = fopen("/", "r");
    int nb_fd = fileno(fd);
    fclose(fd);

    return (nb_fd > 3);
}

/*
 * Detect GDB by the mean of /proc/$PID/cmdline, which should no be "gdb"
 */
int dbg_cmdline(){
    char buff [24], tmp [16];
    FILE* f;

    snprintf(buff, 24, "/proc/%d/cmdline", getppid());
    f = fopen(buff, "r");
    fgets(tmp, 16, f);
    fclose(f);

    return check_strings(tmp);
}

/*
 * Classic self ptrace trick: a program can only be ptraced by ONE other.
 */
int dbg_ptrace(){
    if(ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
        return 1;
    ptrace(PTRACE_DETACH, 0, 0, 0);
    return 0;
}

/*
 * GDB handle SIGTRAP, if we raise one under GDB, it will not reach our handler.
 * FIXME
 */
unsigned int no_dbg = 1;
static void handler(int signum){no_dbg = 0;}
int dbg_sigtrap(){
    no_dbg = 1;
    signal(SIGTRAP, handler);
    raise(SIGTRAP);
    signal(SIGTRAP, SIG_IGN);
    return no_dbg;
}

/*
 * Scan memory for breakpoints
 * TODO
 */
int dbg_check_bp(){
    unsigned char* start;
    size_t size;
    size_t i;

    for(i = 0; i < size; i++){
        if(start[i] == 0xCC || start[i] == 0xCD)
            return 1;
    }
    return 0;
}

/*
 * Check the parent's name
 */
int dbg_getppid_name(){
    char buff1[24], buff2[16];
    FILE* f;

    snprintf(buff1, 24, "/proc/%d/status", getppid());
    f = fopen(buff1, "r");
    fgets(buff2, 16, f);
    fclose(f);

    return check_strings(buff2);
}

