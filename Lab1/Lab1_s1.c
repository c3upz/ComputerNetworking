/*
    Name: Chris Gerbino
    Date: April 1st, 2020
    Title: Lab1 - COEN146L
    Description: This program exemplfies how to use pthread.

    !NOTE: to compile a program that uses pthreads use: "gcc thread.c -o thread -lpthread"
*/

#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */ 
#include <pthread.h>    //using instead of "fork()"
/* main function with command-line arguments to pass */

void *start_routine(void *ptr); //declaring function

int main(int argc, char *argv[]) {
    //pid_t  pid;
    
    pthread_t threadOne;
    int i, n = atoi(argv[1]); // n microseconds to input from keyboard for delay
    printf("\n Before forking.\n");
    //pid = fork();

    pthread_create(&threadOne, NULL, start_routine, &n); //here 'n' holds the user inputed delay?   
    
    // Child process
    for (i=0;i<100;i++) {
        printf("Child process %d\n",i);
        usleep(n);
    
    }
    pthread_join(threadOne, NULL);
    return 0;
}

void *start_routine(void *delayTime){
    // Parent process
    int i;
    for (i=0;i<100;i++) {
        printf("\t \t \t Parent Process %d \n",i);
        usleep(*(int*)delayTime);
    }
}


