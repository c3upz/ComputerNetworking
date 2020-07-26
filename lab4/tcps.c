/*
    NAME: Chris Gerbino
    DUE-DATE: 28 April 2020
    TITLE: Lab4 - Step 1 : Server
    DESCRIPTION: This program implements the server side of a TCP connecton. This
        server can handle requests from multiple clients at once. I have defined
        the number of connections as a constant. This value can be changed to allow
        for more connections.
*/

//TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // this library is needed to use "pthread_t"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>



//constants
#define numberOfConnections 5 //N represents the number of connectionFileDescriptors.. also remember not to add semicolon after defining constant lol

pthread_t threadID[numberOfConnections];

void *start_routine(void *ptrconnectionFileDescriptor); //the argument passed to start_routine will be the connectionFileDescriptor to the client

//need this file pointer to be global becuase it will be used in "start_routine" function and "main" function
FILE *writeToFile;


int main(){
    //Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
    int sockfd, sin_size;
    int *connectionFileDescriptor;
    
    //Declare receiving and sending buffers of size 1k bytes
    char recBuf[1024], sendBuf[1024];

    //Declare server address to which to bind for receiving messages and client address to fill in sending address
    struct sockaddr_in servAddr, clienAddr;

    //Open a TCP socket, if successful, returns  a descriptor  associated  with  an endpoint
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Failure to setup an endpoint socket");
        exit(1);
    }

    //Setup the server address to bind using socket addressing structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5000);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    //Set address/port of server endpoint for socket socket descriptor
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
        printf("Failure to bind server address to the endpoint socket");
        exit(1);
    }

    printf("Server waiting for client at port 5000\n");

    // Server listening to the socket endpoint, and can queue 5 client requests
    listen(sockfd, numberOfConnections);
    //sin_size = sizeof(struct sockaddr_in);

    int i;
    for(i = 0; i <= numberOfConnections; i++){
            int *connectionFileDescriptor = malloc(sizeof(int));
       // connectionFileDescriptor = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size);
    
    //Server accepting to establish a connectionFileDescriptor with a connecting client, if successful, returns a connectionFileDescriptor descriptor  
            //connectionFileDescriptor is malloced
        *connectionFileDescriptor = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size);
        if ((connectionFileDescriptor) < 0){ //accept() returns -1 if error occurs while trying to connect
                                                                                                //accept() returns file descriptor for the accepted soccet if there is no error
            perror("Failure to accept connection to the client");
            exit(1);
        }
        pthread_create(&threadID, NULL, start_routine, connectionFileDescriptor); //creating thread and passing 'connectionFileDescriptor' as argument to start_routine

    }

    fclose(writeToFile);
    close(sockfd);
    free(connectionFileDescriptor); 
    return 0;
}

void *start_routine(void *connectionFileDescriptor){
    char recBuf[1024];
    int numberOfRecBytes =0;
    int fileDescriptor = *(int*)connectionFileDescriptor; // may need to cast 'connectionFileDescriptor'

    while ((numberOfRecBytes = recv(fileDescriptor, recBuf, 1024, 0))>0){
		//this is where i need to write to the file
        writeToFile = fopen("CopiedFileServerTCPtest.txt", "a");
        if(writeToFile == NULL){
            printf("Could not open the file for global writing");
            exit(0);
        }

        if(!feof(writeToFile)){
            fwrite(recBuf, 1, numberOfRecBytes, writeToFile); //wrting 1 byte to file for each recieved byte
        }else{
            printf("EOF reached?");
        }
    }
    //close(connfd);
    if(fflush(writeToFile) == EOF){ //fflush returns 0 if successfull, or EOF if an error occured
        printf("flush unsuccessful"); 
    }
    printf("The server should have recieved the message");
}
