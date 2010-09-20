#include <stdio.h>
#include "spiller.h"

void
my_scanner(u_char *args, const struct pcap_pkthdr *header,
	   const u_char *packet){
  char *data;
  data = start_raw(header, packet);
  printf("parsed data %s\n",data);
}

int
main(int argc, char *argv[]){
  char *dev;
  if(argc>1){
    /* MAIN BLOCK */
    dev = argv[1];
    start_listen(dev,"port 5060", my_scanner);
  }
  else{
    /* some errors in calling apps */
    printf("check for syntax you miss something\n");
  }
}
