#include <stdlib.h>
#include <stdio.h>

// #define SIZE 10

typedef struct {
	int seq_ack;		// Will always be 0 or 1
	int length;		// How many bytes of data you have
	int checksum;		// Checksum calculated by XORing bytes
}HEADER;

typedef struct {
	HEADER header;
	char data[10]; // size = 10
}PACKET;

int getChecksum(PACKET *packet, int num_bytes) {
	char cksum = 0;
	char *pkt = (char*)packet;
	int i = 0;
	for(i = 0; i < num_bytes; ++i) {
		cksum = cksum ^ *pkt;
		++pkt;
	}	
	cksum = ((int)(cksum));
	return cksum;
}
PACKET* createdefault(int seq) {
    HEADER header = {seq,0,0};

    PACKET* packet = malloc(sizeof(packet));
    packet->header = header;

    return packet;
}


PACKET* make_name_packet(char name[], int len, int seq) {
    PACKET* packet = createdefault(seq);
    packet->header.length = len;
    strncpy(packet->data, name, len);
    packet->header.checksum = getChecksum(packet, sizeof(HEADER) + packet->header.length);
    return packet;
}

PACKET* make_packet(FILE* file, int seq) {
    PACKET* pkt = createdefault(seq);
    if (file != NULL) {
        pkt->header.length = fread(pkt->data, 1, 10, file);
        pkt->header.checksum = getChecksum(pkt, sizeof(HEADER) + pkt->header.length);
        return pkt;
    } else {
        pkt->header.length = 0;
        memset(pkt->data, '\0', sizeof(pkt->data));
        pkt->header.checksum = getChecksum(pkt, sizeof(HEADER) + pkt->header.length);
        return pkt;
    }


    return 0;
}


