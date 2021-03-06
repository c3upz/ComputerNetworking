/*
  NAME: Chris Gerbino
  TITLE: Lab6 - client
  DESCRIPTION: This is program that implements rdt3.0 on top of UDP. This file contains the clientside code.
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "lab6.h"




int main (int argc, char *argv[]){
	//char buf[10];
	//int buf = 10;
	int sock, portNum;
	size_t nBytes = 0;
	struct sockaddr_in address;
	socklen_t addr_size;

	struct timeval tv; 	// timer
	int returned_value; // select returned value
	fd_set readfds;

	if (argc != 5) {
	  fprintf(stderr, "Usage: %s <ip> <port> <infile> <outfile>\n", argv[0]);
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons (atoi (argv[2])); //1
	inet_pton (AF_INET, argv[1], &address.sin_addr.s_addr); //argv[1] = ip address
	memset (address.sin_zero, '\0', sizeof(address.sin_zero));  
	addr_size = sizeof(address);

	// create socket
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("\nsocket error");
		return 1;
	}

	FILE *source_file = fopen(argv[3], "rb");
	if(source_file == NULL){
		printf("\nerror opening input file... clientside");
		return 1;
	}
	int seq = 0;

  //create packets
	PACKET *packet = malloc(sizeof(PACKET));
	PACKET *response = malloc(sizeof(PACKET));

	fcntl (sock, F_SETFL, O_NONBLOCK);

	int send_attempts = 0;
	int send_attempts2 = 0;
	int name = 1;
	int counter_flag = 0;
	int len = 10;
	int length = 0;

	while(len >= 1) {
		if(name == 1){ //sending filename
			// HEADER h = {seq, 0, 0,};
			// packet->header = h;
			// packet->header.length = len;
			// len = strlen(argv[4]);
			// strncpy(packet->data, argv[4], length);
			// packet->header.checksum = getChecksum(packet, packet->header.length + sizeof(HEADER));
			packet = make_name_packet(argv[4], strlen(argv[4]), seq);
			name = 0;
		}else{ //filename sent, now sending files content
			// HEADER h = {seq, 0, 0};
			// packet->header = h;
			// if(source_file){//making sure there is data to send
			// 	packet->header.length = fread(packet->data, 1, 10, source_file); //10 is size 
			// }else{
			// 	packet->header.length = -1; // dont need to send anymore data
			// 	memset(packet->data, '\0', sizeof(packet->data));
			// }
			packet = make_packet(source_file, seq);
			len = packet->header.length;
		}

	
    int rand_num = 0;
		//int first_try = 1;
		//while((response->header.seq_ack != seq)||(first_try == 1)) {
		do {
			// random chance of sending incorrect checksum
			rand_num = (rand()%100);
			if(rand_num < 6) {
				printf("\nmimic bad checksum");
				packet->header.checksum = 0;
			}else{
				packet->header.checksum = getChecksum(packet, packet->header.length + sizeof(HEADER));
			}
			FD_ZERO (&readfds);
			FD_SET (sock, &readfds);
			// seting timer
			tv.tv_sec = 1;
			tv.tv_usec = 0;
		
			sendto(sock, packet, sizeof(PACKET), 0, (struct sockaddr *)&address, addr_size);
			returned_value = select(sock + 1, &readfds, NULL, NULL, &tv); 
						

      if(returned_value == 0){
				if(send_attempts >= 3){ //timeout, no data							
					printf("\ntried to send 3 times, but did not send");
					return 1;
				}
				printf("\nresending for %dth time", send_attempts);
				send_attempts++;
				continue;

			}
			send_attempts = 0;
			// has sent w/in 3 tries so now wait for response from server
			recvfrom(sock, response, sizeof(PACKET), 0, NULL, NULL); 
			// recvfrom(sock, response, sizeof(PACKET), 0, (struct sockaddr *)&address, &addr_size); 
      //believe that I only need to check ACK now
			if(packet->header.seq_ack != seq) { 	// If the server returns a correct ack
				printf("bad seq num");
			}
			if(response->header.seq_ack != seq && packet->header.length ==0){
				send_attempts2++;
			}
			if(response->header.seq_ack != seq && packet->header.length ==0 && send_attempts2 >= 3){
				break;
			}
		//}
		} while((response->header.seq_ack != seq));
		//first_try = 0;
		seq = (seq+1)%2; // flipping seq number

	}

	fclose(source_file);
	
	return 0;
}

