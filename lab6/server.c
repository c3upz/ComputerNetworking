/*
  NAME: Chris Gerbino
  TITLE: Lab6 - server
  DESCRIPTION: This is program that implements rdt3.0 on top of UDP. This file contains the serverside code.
*/
/*
Please answer the following questions
        [Q1] Briefly explain what is "AF_INET" used for.
          to assign types of addresses that socket can communicate with (eg. IPv4, IPv6)
        [Q2] Briefly explain what is "SOCK_DGRAM" used for.
          sending indiv. packets w/ ADDRESSES over an unreliable channel
        [Q3] Briefly explain what is "htons" used for.
          translates port numbers from host order to network order...
        [Q4] Briefly explain why bind is required on the returned_value and not on the client.
*/

#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "lab6.h"
#include <sys/socket.h>


int main (int argc, char *argv[]){
	int sockfd, n; //n = # of bytes
	struct sockaddr_in address, clientAddr;
	struct sockaddr_storage storage;
	socklen_t addr_size, client_addr_size;
	int cksum = 0;

  if (argc != 2){
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);        
		return 1;
  }


	address.sin_family = AF_INET;
	address.sin_port = htons ((short)atoi (argv[1]));
	address.sin_addr.s_addr = htonl (INADDR_ANY);
	memset ((char *)address.sin_zero, '\0', sizeof (address.sin_zero));  
	addr_size = sizeof (storage);
	PACKET *packet = malloc(sizeof(packet));

	// creating socket
	if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket error\n");
		return 1;
	}

	// bind
	if (bind (sockfd, (struct sockaddr *)&address, sizeof (address)) != 0){
		perror("\n error when binding...");
		return 1;
	}

	int rlength = 10;
	FILE* dest_file; 
	int size = 10;
	int recv_flag = 1;
  int random = 0;
	while(1) {
		recvfrom(sockfd, packet, sizeof(*packet), 0, (struct sockaddr *)&storage, &addr_size);
		printf("\nrecv packet from cli");
    random = (rand()%100);
		if(random < 3){ //number can change based on how often we want ack msg to be skipped
			printf("ACK was skipped.... random < 3");
			continue; 
		}


		n = packet->header.checksum;
		packet->header.checksum = 0;
		cksum = getChecksum(packet, sizeof(HEADER) + packet->header.length);
		rlength = packet->header.length;

		if(cksum != n) {
			packet->header.seq_ack = (packet->header.seq_ack + 1)%2; //flip sequence number
			printf("\nincorrect checksum");
		} else if(recv_flag) {
				dest_file = fopen(packet->data, "wb");
  			if(dest_file == NULL){
        	printf("\nError opening file..serverside");
       		return 1;
    		}
				recv_flag = 0;
		} else if((rlength) > 0){ // write if data available
				fwrite(packet->data, 1, packet->header.length, dest_file);
		}
	
		sendto(sockfd, packet, sizeof(PACKET), 0, (struct sockaddr *)&storage, addr_size);

		if(n == cksum){
			if(recv_flag == 0){
				if(rlength == 0){
					printf("finished receiving file");
					break;
				}
			}
		}
	}
	

	fclose(dest_file);
	return 0;
}