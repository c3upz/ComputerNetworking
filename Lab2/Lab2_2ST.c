/*
COEN146 Lab #2 Step 4

Name: Chris Gerbino
Date: 4 / 10 / 20
File description: This program calculates network delays without regard for transmission delay
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//NOTE: RTT short for round-trip-time
#define RTT0 3
#define RTT1 20
#define RTT2 26
#define RTTHTTP 47

//added for part b
#define objects 6

int main(){
    //A
    int clickAndRecieveTime = 2 * RTTHTTP + RTT0 + RTT1 + RTT2;
    printf("part a: %d ms", clickAndRecieveTime);

    //B
    int clickAndRecieveTime2 = clickAndRecieveTime + 2 * objects * RTTHTTP; 
    printf("part b: %d ms", clickAndRecieveTime2);

    return 0;
}