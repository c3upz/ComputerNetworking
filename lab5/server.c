/*
  NAME: Chris Gerbino
  DUE-DATE: 11 March 2020
  TITLE: Lab5 - client
  DESCRIPTION: This is program that implements rdt2.2 on top of UDP. This program acts as the server.
*/



#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab5.h"



// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

    while (ptr < end) {
        checksum ^= *ptr++; //same as other file.. 'ptr++' -> '*ptr++'
    }

    return checksum;
} //g

void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout); //should i change my other fwrite statments to something similar? try first then decide
    printf("\" }\n");
} //g

//Sending a ACK to the client, i.e., letting the client know what was the status of the packet it sent.
void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;
	//-Prepare the packet headers
	//-send the packet
    	//******STUDENT WORK******
	packet.header.seq_ack = seqnum;
    packet.header.len = sizeof(packet.data);
    packet.header.cksum = getChecksum(packet);
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

    printf("Sent ACK %d, checksum %d\n", (packet.header.seq_ack), (packet.header.cksum));
} //g

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;

    while (1) {
        //recv packets from the client
		//******STUDENT WORK******
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // log what was received
        printf("Received: ");
        logPacket(packet);

        //******STUDENT WORK******
        if (packet.header.cksum != getChecksum(packet)) { // if checksum didnt match
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(packet));
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) { // if sequence number didnt match
            printf("Bad seqnum, expected sequence number was:%d\n", seqnum);
            	//******STUDENT WORK******
            ServerSend(sockfd, address, *addrlen, !seqnum); //same as above
        } else { // if sequence number and checksum match we are good to go!
            printf("Good packet\n");
            	//******STUDENT WORK******
            ServerSend(sockfd, address, *addrlen, seqnum); //same as above
            break;
        }
    }

    printf("\n");
    return packet;
} //g


int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
	//******STUDENT WORK******
    int sockFileDescriptor;
    if ((sockFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket\n");
        exit(1); //exit(1) bc error
    }
    // initialize the server address structure using argv[1]
    struct sockaddr_in address;
	//******STUDENT WORK******
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
     if ((bind(sockFileDescriptor, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0){
        perror("Failure to bind server address to the endpoint socket\n");
        exit(1);
    }
    
        /* data */
    
	// [Question]: Do we need to bind the socket on the client side? Why?/Why not?
	//  No we should not need to bind the socket on the client side. This is because the connection is unidirectional (client -> server).

    // open file using argv[2]
	//******STUDENT WORK******
    FILE *filePointer; 
    filePointer = fopen(argv[2], "wb");
    if(filePointer == NULL){
        printf("Error opening file..serverside");
        exit(1);
    }


    // get file contents from client
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;

    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
	//******STUDENT WORK******
        packet = ServerReceive(sockFileDescriptor, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
        fwrite(packet.data, 1, ((int)(sizeof(packet.data))), filePointer);
        printf("%d", (int)(sizeof(packet.data)));
        seqnum++;
        seqnum = seqnum % 2;
    } while (packet.header.len != 0);

    fclose(filePointer);
    close(sockFileDescriptor);
    return 0;
}
