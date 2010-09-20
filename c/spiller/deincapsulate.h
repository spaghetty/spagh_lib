#ifndef __DEINCAPSULATE___
#define __DEINCAPSULATE___

#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap.h>

#define udp_packet_size 8

typedef struct ether_header ether_t;

int dlink_type;
int dlink_size;

void finally_sip(const u_char *packet);

char *handle_udp(const u_char *packet);

char *handle_tcp(const u_char *packet);

char *handle_ip(const u_char *packet);

char *andle_ether(const u_char *packet);

char *start_raw(const struct pcap_pkthdr *header, const u_char *packet);

void setup_dlink(pcap_t *session);

#endif /* __DEINCAPSULATE___ */
