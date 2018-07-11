#include "pp2p.h"
#include "fairp2p.h"

Message *delivered;
Message *sent;
int numberMessages, numberDelivered, sendPort, recvPort;

// wrapper for perror
void error(char *msg) {
  perror(msg);
  exit(1);
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


// StubbornP2P
// ########################################################

//ports to receive and send messages
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

// ########################################################

// PerfectP2P

void *pp2pthread(void * arg){
    delivered = malloc(sizeof(Message)*BUFSIZE);

    pthread_exit(NULL);
}

void pp2pSend(char *dest, char*msg){
    stp2psend(dest, msg);
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