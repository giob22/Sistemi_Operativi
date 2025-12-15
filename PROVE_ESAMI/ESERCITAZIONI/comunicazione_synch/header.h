#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define DIM_MSG 40

#define RTS 1
#define OTS 2
#define MESSAGGIO 3

#ifdef DEBUG
#define master(s, ...) printf("[master] " s "\n", ##__VA_ARGS__)
#define cons(s, ...) printf("[cons] " s "\n", ##__VA_ARGS__)
#define prod(s, ...) printf("[prod] " s "\n", ##__VA_ARGS__)

#endif


typedef char msg[DIM_MSG];


typedef struct{
	long type;
}ack_msg;

typedef struct{
	long type;

	msg text;
}data_msg;




void init_synch();
void leave_synch();


void send_synch(data_msg*, int);
void receive_synch(data_msg*, int, long);

void produci(int, char*);
void consuma(int);

