#include "pp2p.h"
#include "stp2p.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Message *delivered;
int numberDelivered, sendPort, recvPort;

void pp2pthread(void * arg){
    delivered = malloc(sizeof(Message)*BUFSIZE);
}

void pp2psend(char *dest, char*msg){
    stp2psend(dest, msg);
}

// Compara para ver se a mensagem já está no array de recebidas
bool isDelivered(char *msg){
    for(int i = 0; i < numberDelivered; i++){
        Message m = delivered[i];

        if(strcmp(msg, m.msg) == 0){
            return true;
        }
    }

    return false;
}

void stp2pDelivery(char *src, char *msg){
    if(!isDelivered(msg)){
        Message m;
        m.addr = src;
        m.msg  = msg;
        delivered[numberDelivered] = m;
        numberDelivered++;
        pp2pDelivery(src, msg);
    }
}

//upon event <sbp2p.Init>
pthread_t pp2pInit(int pp2psendPort, int pp2precvPort) {
    sendPort = pp2psendPort; //port to send messages to dst nodes
    recvPort = pp2precvPort; //port to receive messages from src nodes
    numberDelivered = 0;
 
    /* create a internal thread to receive and delivery messages */
    pthread_t tid;
    if(pthread_create(&tid, NULL, pp2pthread, NULL))
       error("error pthread_create()");
    return tid;
}