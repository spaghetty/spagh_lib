#include <stdio.h>
#include "spiller.h"

char errbuf[PCAP_ERRBUF_SIZE];

void
got_packet(u_char *args, const struct pcap_pkthdr *header,
	   const u_char *packet){
  start_raw(header, packet);
}


int
start_listen(char *dev){
  pcap_t *session;
  session = pcap_open_live(dev, BUFSIZ, 0, 10 , errbuf);
  if (session == NULL){
    printf("%s\n",errbuf);
    return 0;
  }
  setup_dlink(session);
  pcap_loop(session, 1000 , got_packet, "");
}

