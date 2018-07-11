#define BUFSIZE 1024
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include <pthread.h>


#define TIMER 30

typedef struct Message
{
   char *addr;
   char *msg;
} Message ;

pthread_t pp2pInit(int pp2psendPort, int pp2precvPort);

void pp2pSend(char *dest, char*msg);

void error(char *msg);

void fp2pDelivery (char *src, char *msg);