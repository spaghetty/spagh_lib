#include <pcap.h>
#include <stdlib.h>
#include "deincapsulate.h"

void finally_sip(const u_char *packet)
{
}

char *handle_udp(const u_char *packet)
{
  struct udphdr *udpp = (struct udphdr *) packet;
  const u_char *next_capsule = packet+udp_packet_size;
  return next_capsule;
}

char *handle_tcp(const u_char *packet)
{
  struct tcphdr *tcpp = (struct tcphdr *) packet;
  const u_char *next_capsule = packet+(tcpp->doff*4);
  return next_capsule;
}

char *handle_ip(const u_char *packet)
{
  struct ip *ipp= (struct ip *) packet;
  const u_char *next_capsule = packet+(ipp->ip_hl*4);
  switch(ipp->ip_p)
    {
    case 1:
      /* ICMP */
      break;
    case 3:
      /* G2G */
      break;
    case 4:
      /* CMCC */
      break;
    case 5:
      /* ST */
      break;
    case 6:
      /* tcp */
      return handle_tcp(next_capsule);
      break;
    case 7:
      /* merda */
      break;
    case 17:
      /* udp */
      return handle_udp(next_capsule);
      break;
    }
}

char *handle_ether(const u_char *packet)
{
  const ether_t *eth;
  eth = (ether_t *)(packet);
  const u_char *next_capsule = packet+dlink_size;
  int type = ntohs(eth->ether_type);
  switch(type)
    {
    case ETHERTYPE_PUP:
#ifdef SNF_DBG
      printf("pup here \n");
#endif
      break;
    case ETHERTYPE_SPRITE:
#ifdef SNF_DBG
      printf("sprite here \n");
#endif
      break;
    case ETHERTYPE_IP:
#ifdef SNF_DBG
      printf("ip here \n");
#endif
      return handle_ip(next_capsule);
      break;
    case ETHERTYPE_ARP:
#ifdef SNF_DBG
      printf("arp here \n");
#endif
      break;
    case ETHERTYPE_REVARP:
#ifdef SNF_DBG
      printf("revarp here \n");
#endif
      break;
    case ETHERTYPE_AT:
#ifdef SNF_DBG
      printf("at here \n");
#endif
      break;
    case ETHERTYPE_VLAN:
#ifdef SNF_DBG
      printf("vlan here \n");
#endif
      break;
    case ETHERTYPE_IPX:
#ifdef SNF_DBG
      printf("ipx here \n");
#endif
      break;
    case ETHERTYPE_IPV6:
#ifdef SNF_DBG
      printf("ipv6 here \n");
#endif
      break;
    case ETHERTYPE_LOOPBACK:
#ifdef SNF_DBG
      printf("loopback here \n");
#endif
      break;
    }
}

char *start_raw(const struct pcap_pkthdr *header,
	       const u_char *packet)
{
  switch(dlink_type)
    {
    case DLT_EN10MB:
      return handle_ether(packet);
      break;
    default:
      printf("unmanagiable dlink type\n");
      break;
    }
}

void setup_dlink(pcap_t *session)
{
  dlink_type = pcap_datalink(session);
  if(dlink_type==-1){
    printf("trouble looking for datalink type\n");
    exit(0);
  }
  switch( dlink_type )
    {
    case DLT_EN10MB:
      dlink_size = 14;
      break;
    case DLT_IEEE802_11:
      dlink_size = 22;
      break;
    case DLT_FDDI:
      dlink_size = 21;
      break;
    case DLT_PPP_ETHER :
      dlink_size = 20;
      break;
    case DLT_NULL:
      dlink_size = 4;
      break;
    case DLT_PPP:
      dlink_size = 0;
      break;
    }
}

