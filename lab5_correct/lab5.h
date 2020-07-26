#ifndef liebermanlab5_h
#define liebermanlab5_h

//Header Struct
typedef struct
{
    int seq_ack;
    int len; //length of the data
    int cksum;
} Header;

//Packet Struct
typedef struct
{
    Header header;
    char data[10];
} Packet;

int get_chksum(Packet);

Packet make_packet(char[], int, int);

#endif
