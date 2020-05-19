#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//funciton declarations

double binomial(double a, double b);
double factorial(double a);

int main (int argc, char *argv[]){
//Values given in the lab
    int linkBandwidth = 200; //bandwidth of a network link
    int userBandwidth = 20;  //bandwidth required for a given user
    double tPSuser = 0.10;     //precentage of time a opacket switching user needs to transmit
    int nPSusers = 19;       //number of packet switching users


    //STEP6
    int nCSusers = linkBandwidth / userBandwidth;

    //STEP7

    //a
    double pPSusers = tPSuser;

    //b
    double pPsusersNotBusy = 1 - pPSusers;

    //c
    double allOtherUsers = pow((1 - pPSusers),(nPSusers-1));

    //d
    double OneTransmitting = pPSusers * pow((pPsusersNotBusy),(nPSusers-10));

    //e
    double exactlyOneBusy = nPSusers * (pPSusers* pow((pPsusersNotBusy),(nPSusers-1)));

    //f
    double tenSpecificUsers = pow((pPSusers),(10)) * pow((pPsusersNotBusy), (nPSusers-10));

    //g
    double coefficient = binomial(nPSusers, 10);
    double anyTen = coefficient * pow((pPsusersNotBusy),(nPSusers - 10));

    //h
    double total;
    double i;

    for(i = 11; i < (nPSusers + 1); i++){
        total += (binomial(nPSusers,i) * pow(pPSusers,i) * pow(pPsusersNotBusy,(nPSusers-i)));
    }

    return 0;

}


double factorial(double a){
    double i;
    double ans;

    for(i=1; i<a;i++){
        ans = (ans * i);
    }
    return ans;
}

double binomial(double a, double b){
    double ans; 
    ans = (factorial(a))/(factorial(b) * factorial((a-b)));
    return ans;
}