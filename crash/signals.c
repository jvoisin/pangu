#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig){
    printf("No debugger, or passed signals\n");

    //Put your code here

    _exit(0);
}

int main(){
    /*
     * Not everyone is clever enough to
     * think that a SIGEGV/SIGILL is rightful ;)
     */

    signal(SIGSEGV, handler);
    __asm__("movl $1, %eax");

    signal(SIGILL, handler);
    __asm__("ud2");
}
