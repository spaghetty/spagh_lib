#ifndef __SPILL__
#define __SPILL__

#include <pcap.h>
#include "deincapsulate.h"

struct bpf_program fp;

//void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

pcap_t *start_listen(char *dev, char *filter_exp, pcap_handler callback);

#endif /* __SPILL__ */
