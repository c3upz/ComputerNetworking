/*
    NAME: Chris Gerbino
    DUE-DATE: 28 April 2020
    TITLE: Lab4 - Part 2 : Server
    DESCRIPTION: This is a UDP server that receives files from the client
*/


// UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(){

   //Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
   int sockfd;

   //Declare receiving buffer of size 1k bytes
   char rbuf[1024];

   //Declare server address to which to bind for receiving messages and client address to fill in sending address
   struct sockaddr_in servAddr, clienAddr;
   socklen_t addrLen = sizeof(struct sockaddr);

   //Open a UDP socket, if successful, returns  a descriptor  associated  with  an endpoint 
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("Failure to setup an endpoint socket");
      exit(1);
   }

   //Setup the server address to bind using socket addressing structure
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = INADDR_ANY; //Local IP address of any interface is assigned (generally one interface IP address)
   servAddr.sin_port = htons(5000); //Port 5000 is assigned... NEED TO REMEMBER PORT # SO I CAN TEST


   //Set address/port of server endpoint for socket socket descriptor
   if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
      perror("Failure to bind server address to the endpoint socket");
      exit(1);
   }

   //Sever continuously waits for messages from client, then prints incoming messages.
   while (1){
      //printf("Server waiting for messages from client: \n");
      //char rbuf[1024];
      int nr = recvfrom(sockfd, rbuf, 1024, 0, (struct sockaddr *)&clienAddr, &addrLen);
      rbuf[nr] = '\0';
      printf("Client with IP: %s and Port: %d sent message: %s\n", inet_ntoa(clienAddr.sin_addr),ntohs(clienAddr.sin_port), rbuf);


      //writing to file here
       //i will write to the file in the while loop below so I need to
      //put my file here
      FILE *writeFile = fopen("test.txt", "wb");
      
      if(!feof(writeFile)){
         fwrite(rbuf, 1, nr, writeFile);
      }else{
         printf("error");
      }

      fclose(writeFile);
   
   }

   return 0;
}
