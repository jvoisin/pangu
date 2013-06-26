/*
 *Excerpt of the bug's description:
    GDB fails to interrupt the program being debugged if the program is blocking SIGINT.

    When using the sigwait function to retrieve signals,
    the program is expected to block them.  SIGINT is a commonly handled signal.

    Any program using sigwait to retrieve signals and handling SIGINT this way will not be interruptible by GDB.

The dectection process used here is the SIGTRAP trick. Fell free to use another one.
 */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void no_gdb(int s){
    signal(SIGTRAP, SIG_DFL);
    printf("[*] No GBD detected\n");
    /*
     * Put your code here
     */
     _exit(0);
}

int main(){
    signal(SIGTRAP, &no_gdb);
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGINT);
    sigprocmask(SIG_BLOCK, &sigs, NULL);

    raise(SIGTRAP);

    printf("[*] GDB detected\n");
    while(1);
    return 0;
}
