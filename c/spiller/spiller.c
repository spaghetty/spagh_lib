#include <stdio.h>
#include "spiller.h"

char errbuf[PCAP_ERRBUF_SIZE];

/*
void
got_packet(u_char *args, const struct pcap_pkthdr *header,
	   const u_char *packet){
  start_raw(header, packet);
}
*/

pcap_t *
start_listen(char *dev, char *filter_exp, pcap_handler callback){
  pcap_t *session;
  bpf_u_int32 mask;
  session = pcap_open_live(dev, BUFSIZ, 0, 10 , errbuf);
  if (session == NULL){
    printf("%s\n",errbuf);
    return 0;
  }
  if (pcap_compile(session, &fp, filter_exp, 0, 0) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(session));
    return 0;
  }
  if (pcap_setfilter(session, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(session));
    return 0;
  }
  setup_dlink(session);
  pcap_loop(session, 1000 , callback, "");
  return session;
}

