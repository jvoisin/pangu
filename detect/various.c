/*
 * Detection of various tricks
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
 * Dead simple try to detect the LD_PRELOAD trick by looking
 * into environnement variables of the program.
 */
int various_ldpreload(){
    return (getenv("LD_PRELOAD") != NULL);
}

/*
 * Try to detect the LD_PRELOAD trick when used with a custom
 * gentenv() function. Feel free to expand it using the same principe.
 */
int various_ldpreload_custom_getenv(){
    if(!various_ldpreload()){
        putenv("LD_PRELOAD=hello");
        return (strcmp(getenv("LD_PRELOAD"), "hello"));
    }
    return 1;
}

/*
 * Timiming attack, useful for detecting breakpoints
 */
int various_rdtsc(){
    clock_t t = clock();

    int i; //Time-consuming crap
    for(i=2; i<7137001; i++)
        if (7137001 % i == 0)
            break;

    return (clock() - t > 0xffff); //adjust this
}

