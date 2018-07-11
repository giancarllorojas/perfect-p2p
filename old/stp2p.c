#include "fairp2p.h"
#include "stp2p.h"
#include <unistd.h>

//ports to receive and send messages
Message *sent;
int numberMessages;
int sendPort, recvPort;

//upon event <fp2p.Deliver | p,m>
void fp2pDelivery (char *src, char *msg) {
  stp2pDelivery(src, msg);
}

void stp2psend(char *dest, char *msg){
  fp2pSend(dest, msg);
  Message m;
  m.addr = dest;
  m.msg = msg;

  sent[numberMessages] = m;
  numberMessages++;
}

void startTimer(){
    sent = malloc(sizeof(Message)*BUFSIZE);
    startTimer(TIMER);
}


void * stp2pthread(void * arg) {
  sent = malloc(sizeof(Message)*BUFSIZE);
  fp2pInit(sendPort, recvPort);

  startTimer();
  pthread_exit(NULL);
}

void Timeout(){
  for(int i=0; i < numberMessages; i++){
    Message m = sent[i];
    fp2pSend(m.addr, m.msg);
  }

  startTimer();
}


//upon event <sbp2p.Init>
pthread_t stp2pInit(int stp2psendPort, int stp2precvPort) {
    sendPort = stp2psendPort; //port to send messages to dst nodes
    recvPort = stp2precvPort; //port to receive messages from src nodes
    numberMessages = 0;
 
    /* create a internal thread to receive and delivery messages */
    pthread_t tid;
    if(pthread_create(&tid, NULL, stp2pthread, NULL))
       error("error pthread_create()");
    return tid;
}