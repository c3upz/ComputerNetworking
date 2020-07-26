/*
    NAME: Chris Gerbino
    DUE-DATE: 28 April 2020
    TITLE: Lab4 - Step 1 : Client
    DESCRIPTION: This file implements the client side for a TCP connection.
*/

//TCP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
//Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors. nr is a declaration of the number of recieved bytes.
int sockfd, nr;

FILE *fileToSend; //this is the file that we will send to the server

//Declare sending and recieving buffers of size 1k bytes
char sendBuf[10], recBuf[1024];

//Declare server address to connect to
struct sockaddr_in servAddr;
struct hostent *host;

//Converts  domain  names   into  numerical  IP  addresses  via  DNS
host = (struct hostent *)gethostbyname("localhost");

//Open a socket, if successful, returns  a descriptor  associated  with  an endpoint
 if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

//Set the server address to send using socket addressing structure
servAddr.sin_family = AF_INET;
servAddr.sin_port = htons(5000);
servAddr.sin_addr = *((struct in_addr *)host->h_addr);

//Connect to the server
if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))){
    perror("Failure to connect to the server");
    exit(1);
}

fileToSend = fopen("test.txt","r"); //later add if statement to catch error

if(fileToSend == NULL){
    printf("Error opening file");
    exit(0);
}

while(!feof(fileToSend)){
    //read a byte from the file 
    int byteFromFile = fread(sendBuf, 1, sizeof(sendBuf), fileToSend);
    //send the byte to the server
    sendto(sockfd, sendBuf, byteFromFile, 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));

}
 //Client begins to write and read from the server
//  while(1){
//   printf("Client: Type a message to send to Server\n");
//   scanf("%s", sbuf);
//   write(sockfd, sbuf, strlen(sbuf));
//   read(sockfd, rbuf, 1024);
//   printf("Server: sent message: %s\n", rbuf);
//  }
 //Close socket descriptor
 close(sockfd);
 return 0;
}
