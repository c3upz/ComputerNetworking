/*
  NAME: Chris Gerbino
  DUE-DATE: 28 April 2020
  TITLE: Lab4 - Part 2 (one peer)
  DESCRIPTION: This is program that implements UDP p2p file transfer
*/

//UDP Peer1
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
#include <unistd.h>

//Essentially both of the p2p programs will need to act as a server and a host

//All of the variables below will need to be global as I will need to use in diff funcs

//Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
int sockFileDescriptor; 

//Declare sending buffer of size 1k bytes, and recieving buffer of same size
char sendBuf[1024], recBuf[1024]; 
char hostName[50]; 

//Declare server address
struct sockaddr_in sendAddr; //address responsible for sending data from this peer
struct sockaddr_in recAddr;  //address responsible for recieving data from other peer

//Converts  domain  names   into  numerical  IP  addresses  via  DNS
struct hostent *host;
socklen_t addrLen = sizeof(struct sockaddr);

//initalize functions
void defineConnection();
void sendDataToPeer();
void retrieveDataFromPeer();


void clearBuf(char* b) { 
    int i; 
    for (i = 0; i < sizeof(recBuf); i++) 
        b[i] = '\0'; 
} 

void defineConnection(){

  if((host = (struct hostent *)gethostbyname(hostName)) == NULL){
    printf("That is not a valid hostname\n");
    exit(1); //exit(1) instead of exit(0) bc an error has occured... not "all is well"
  }
  //setting address for sending
  sendAddr.sin_family = AF_INET;
  sendAddr.sin_port = htons(5001);
  sendAddr.sin_addr = *((struct in_addr *)host->h_addr);
  //setting address used for recieving
  recAddr.sin_family = AF_INET;
  recAddr.sin_port = htons(5002);
  recAddr.sin_addr = *((struct in_addr *)host->h_addr);

  if ((sockFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket\n");
    exit(1);
  }
}

void sendDataToPeer(){
  int numberOfRecBytes;

  gethostname(hostName, 50);

  //displaying hostname
  printf("Sending Peer's Hostname: %s \n", hostName);

  //define connection
  defineConnection();

  if ((bind(sockFileDescriptor, (struct sockaddr *)&sendAddr, sizeof(struct sockaddr))) < 0){
    perror("Failure to bind server address to the endpoint socket\n");
    exit(1);
  }
  
  //Now the sending peer is waiting for the receiving peer to send a request for a file
  while ((numberOfRecBytes = recvfrom(sockFileDescriptor, recBuf, sizeof(recBuf), 0, (struct sockaddr*)&recAddr, &addrLen)) > 0){
    
      printf("other peer response: %s \n", recBuf); //the data in recBuf will tell us whether the client wants the file sent of not

      if(strcmp("ACK", recBuf) == 0){
        printf("other peer wants the file to be sent\n");
        break;
      }else{
        printf("other peer does not want the file...\n");
      }
  }

  clearBuf(recBuf); //clearing the receiving buffer so no complications later

  FILE *filePointer;
  filePointer = fopen("SherlockTestDoc.txt", "r");

  while(!feof(filePointer)){
    int temp = fread(sendBuf, 1, sizeof(sendBuf), filePointer);
    sendto(sockFileDescriptor, sendBuf, temp, 0, (struct sockaddr *)&recAddr, sizeof(struct sockaddr));
  }

  clearBuf(sendBuf);
  fclose(filePointer);
  close(sockFileDescriptor);
}

void retrieveDataFromPeer(){
  printf("What is the hostname of the peer you would like to receive the file from?\n");
  printf("hostname: ");
  scanf("%s", hostName);

  defineConnection();
  clearBuf(sendBuf);
  //now that connection is defined I can send send ack that we want file to other peer
  printf("ready to send ACK for file..\n");
  sendto(sockFileDescriptor, "ACK", sizeof("ACK"), 0, (struct sockaddr *)&recAddr, sizeof(struct sockaddr));
  printf("ACK sent!\n");

  //now we need to prepare for response
  clearBuf(recBuf);
  int numberOfRecBytes = 0;
  FILE *writeToFile;
  
  while ((numberOfRecBytes = recvfrom(sockFileDescriptor, recBuf, sizeof(recBuf), 0, (struct sockaddr*)&recAddr, &addrLen)) > 0){
    writeToFile = fopen("Demo4.txt", "w");
    if(writeToFile == NULL){
        printf("Could not open the file for global writing");
        exit(0);
    }

    if(!feof(writeToFile)){
        fwrite(recBuf, 1, numberOfRecBytes, writeToFile); //wrting 1 byte to file for each recieved byte
    }else{
        printf("EOF reached?");
    }
    if(fflush(writeToFile) == EOF){ //fflush returns 0 if successfull, or EOF if an error occured
        printf("flush unsuccessful"); 
    }
    printf("The server should have recieved the message");
  }

  close(sockFileDescriptor);
}


int main(){

  while(1){
    printf("Hello Peer1! Please select an action\n");
    printf("1: Send a file to the other peer\n");
    printf("2: Recieve a file to the other peer\n");
    printf("3: Exit the program\n");

    int userSelection = 0;
    scanf("%d", &userSelection);

    if(userSelection == 1){
      printf("You have chosen to send a file to the other peer!\n");
      //Steps:
        //define connection: set addresses, ports, and retrieve file descriptor
        //initialize sending
        //wait for other peer to select to recieve files
        //sendfiles
      sendDataToPeer();

    }else if(userSelection == 2){
      printf("You have chosen to receive a file to peer2!\n");
      //Steps: 
        //define connection: set addresses, ports, and retrieve file descriptor
        //acknowledge other peer
        //receive file
        //write file locally
      retrieveDataFromPeer();

    }else if(userSelection == 3){
      printf("Goodbye!");
      break;
    }else{
      printf("Please choose a valid selection\n");
    }
  }

  return 0;
}
