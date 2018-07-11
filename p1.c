/* Application to experiment the FairP2P module */
#include "pp2p.h"

#define SENDPORT 3001
#define RECVPORT 4001

// !!! Must be implemented to handle the indication event !!!
void pp2pDelivery (char *src, char *msg) {
  printf("Recebi: %s %s\n", src, msg);
}   

// main function
int main(int argc, char **argv) {
  char msg[BUFSIZE];
  pthread_t tid; 
 
  if(argc < 2) 
     error("Digite <p1> <ip dest>");

  tid = pp2pInit(SENDPORT, RECVPORT);

  for(int i=0;i<20; i++) {
     sprintf(msg, "Msg: %d", i);
     pp2pSend(argv[1], msg);
  }

  pthread_join(tid, NULL);
}
