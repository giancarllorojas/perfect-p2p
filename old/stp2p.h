#define BUFSIZE 1024
#define TIMER 30
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

pthread_t stp2pInit(int stp2psendPort, int stp2precvPort);
void stp2psend(char *dest, char *msg);

typedef struct Message
{
   char *addr;
   char *msg;
} Message ;