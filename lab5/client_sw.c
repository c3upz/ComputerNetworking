#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include "lab5.h"


// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Please find an error from the remaining part getChecksum() function 
//******STUDENT WORK****** 

    while (ptr < end) {
        checksum ^= *ptr++; //changed 'ptr++' to '*ptr++'.. from error
    }

    return checksum;
} //should be done with this section g

void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
} //g



void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {

        // send the packet
        printf("Sending packet\n");
		//******STUDENT WORK******
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // receive an ACK from the server
        Packet recvpacket;
		//******STUDENT WORK******
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);

        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        // validate the ACK... dont need to resend inside statements below bc loop wont break until correct ACK recieved
        if (recvpacket.header.cksum != getChecksum(recvpacket)) {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket));
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack) { //seq numbers arnt the same
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack); //expected seq #
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
            break;
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create and configure the socket [name the socket sockfd] 
	//******STUDENT WORK******	    
    int sockFileDescriptor;
    if ((sockFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket\n");
        exit(1); //exit(1) bc error
    }


    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
	//******STUDENT WORK******
    struct hostent *host;
    socklen_t addrLen = sizeof(address);

    if((host = ((struct hostent *)gethostbyname(argv[1]))) == NULL){ //argv[1] holds hostname
        printf("Hostname not valid\n");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2])); //argv[2] holds
    address.sin_addr = *((struct in_addr *)host->h_addr);

    // open file using argv[3]
	//******STUDENT WORK******
    int filePointer;
    filePointer = open(argv[3], O_RDWR);
    

    // send file contents to server
	//-create a packet
    Packet packet;
	//-set the seq_ack field 
	//-set the len field
	//-calculate the checksum
	//-send the packet
	//******STUDENT WORK******
    int seqnum = 0;
    //fread(packet.data, 1, sizeof(packet.data), filePointer)
    
    while(read(filePointer, packet.data, sizeof(packet.data)) > 0){ // Reads from file then sends to server
        //printf("Client sending file contents to server...\n")
        packet.header.seq_ack = seqnum;
        packet.header.len = strlen(packet.data);
        packet.header.cksum = getChecksum(packet);
        ClientSend(sockFileDescriptor, (struct sockaddr *)&address, addrLen, packet);
        seqnum++;
        seqnum = (seqnum % 2);
    }



    // send zero-length packet to server to end connection
	//******STUDENT WORK******
    Packet endPacket;
    endPacket.header.seq_ack = seqnum;
    endPacket.header.len = 0; //length = 0;
    ClientSend(sockFileDescriptor, (struct sockaddr *)&address, addrLen, endPacket);


    // clean up
	//-close the sockets and file
    	//******STUDENT WORK******
    close(filePointer);
    close(sockFileDescriptor);


    return 0;
}
