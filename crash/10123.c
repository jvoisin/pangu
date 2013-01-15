/*
 *Excerpt of the bug's description:
 GDB fails to interrupt the program being debugged if the program is blocking SIGINT.

 When using the sigwait function to retrieve signals, the program is expected to block them.  SIGINT is a commonly handled signal.  Any
 program using sigwait to retrieve signals and handling SIGINT this way will not be interruptible by GDB.
 */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(){
    sigset_t sigs;
    sigfillset(&sigs);
    sigprocmask(SIG_SETMASK, &sigs, NULL);

    if(fork()){
        sleep(1); // to be sure that
        kill(getppid(), SIGINT);
        _exit(0);
    }
    while(1){
        pause();
        printf("[*] No GBD detected\n");
        /*
         * Put your code here
         */
    }
    return 0;
}
