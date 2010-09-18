#ifndef __SPILL__
#define __SPILL__

#include <pcap.h>
#include "deincapsulate.h"

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int start_listen(char *dev);

#endif /* __SPILL__ */
