#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "list.h"
#include "skel.h"

struct route_table_entry *rtable;
int rtable_len;

struct arp_entry cache[100];
int cache_size;
queue unsend_packets;

#define ARPHRD_IP 0x0800
#define IP_ALEN 4

/*---------------------------------------------------------------------------------------------------------------*/
/*comparator function for sorting rtable (in descending order based on netmask)*/
int comp_rtable_desc(const void * elem1, const void * elem2) 
{
    struct route_table_entry f = *((struct route_table_entry*)elem1);
    struct route_table_entry s = *((struct route_table_entry*)elem2);
    if (ntohl(f.mask) > ntohl(s.mask)) return -1;
    if (ntohl(f.mask) < ntohl(s.mask)) return  1;
    return 0;
}

/*finds the first matching entry in rtable or NULL*/
struct route_table_entry *lpmatch(uint32_t dest_ip) {
	int idx = -1;

	for (int i = 0; i < rtable_len; i++) {
		if ((dest_ip & rtable[i].mask) == rtable[i].prefix) {
			if (idx == -1 || ntohl(rtable[idx].mask) < ntohl(rtable[i].mask)) {
				idx = i;
			}
		}
	}

    if (idx == -1)
        return NULL;
    else
        return &rtable[idx];
}

/*---------------------------------------------------------------------------------------------------------------*/

/*gets an ip's mac by searching in cache*/
int get_mac_from_ip(uint32_t ip, uint8_t *mac_buffer) {
	uint8_t mac_zero[6];
	memset(mac_zero, 0, 6);
	for(int i = 0; i <= cache_size; i++) {
		if (cache[i].ip == ip) {
			if (!memcmp(cache[i].mac, mac_zero, 6)) {
				return -1;
			}
			fprintf(stdout, " found\n");
			memcpy(mac_buffer, cache[i].mac, 6);
			return 0;
		}
		fprintf(stdout, "\n");	
	}
	return 1;
}

/*---------------------------------------------------------------------------------------------------------------*/

/*generates an arp packet*/
void generate_ARP_packet(packet *m, int interface, uint8_t *dest_mac, uint32_t dest_ip, int arp_op) {
	packet arp_m;
	struct ether_header *arp_eth = (struct ether_header *)arp_m.payload;
	struct arp_header *arph = (struct arp_header *)(((void *) arp_eth) + sizeof(struct ether_header));

	uint8_t src_mac[6];
	get_interface_mac(m->interface, src_mac);

	struct in_addr src_ip;
	inet_aton(get_interface_ip(m->interface), &src_ip);

	//PACKET INFO
	arp_m.interface = interface;
	arp_m.len = sizeof(struct ether_header) + sizeof(struct arp_header);

	//ETH HEADER

	//package type
	arp_eth->ether_type = htons(ETHERTYPE_ARP);
	//source mac address
	memcpy(arp_eth->ether_shost, src_mac, 6);
	//destination mac address
	memcpy(arp_eth->ether_dhost, dest_mac, 6);

	//ARP HEADER

	// ARP opcode (command)
	arph->op = htons(arp_op);
	//format of hardwere address
	arph->htype = htons(ARPHRD_ETHER);
	//format of protocol address
	arph->ptype = htons(ARPHRD_IP);
	//length of hardwere address
	arph->hlen = ETH_ALEN;
	//length of protocol address
	arph->plen = IP_ALEN;
	//source mac address
	memcpy(arph->sha, src_mac, 6);
	//destination ip address
	arph->tpa = dest_ip;
	//destination mac address (broadcast)
	memcpy(arph->tha, dest_mac, 6);

	//sender ip address
	arph->spa = src_ip.s_addr;


	send_packet(&arp_m);
	if (arp_op == ARPOP_REPLY) {
		fprintf(stdout, "replied ARP\n");
	} else if (arp_op == ARPOP_REQUEST) {
		fprintf(stdout, "requested ARP\n");
	}

	cache[cache_size].ip = arph->tpa;
	memset(cache[cache_size].mac, 0, 6);
	cache_size++;
}

/*---------------------------------------------------------------------------------------------------------------*/

/*generates an icmp packet*/
void generate_ICMP_packet(packet *m, uint8_t type, uint8_t code, uint8_t *src_mac, uint32_t src_ip) {
	packet icmp_m;
	struct ether_header *icmp_eth = (struct ether_header *)icmp_m.payload;
	struct ip *icmp_iph = (struct ip *)((void *)icmp_eth + sizeof(struct ether_header));
	struct icmp *icmph = (struct icmp *)((void *)icmp_iph + sizeof(struct ip));

	struct ether_header *eth = (struct ether_header *)m->payload;
	struct ip *iph = (struct ip *)((void *)eth + sizeof(struct ether_header));

	//PACKET INFO
	icmp_m.interface = m->interface;
	icmp_m.len = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct icmp);

	//Ether header
	icmp_eth->ether_type = htons(ETHERTYPE_IP);
	memcpy(icmp_eth->ether_shost, src_mac, 6);
	memcpy(icmp_eth->ether_dhost, eth->ether_shost, 6);

	// IP header
	icmp_iph->ip_v = 4;
    icmp_iph->ip_hl = 5;
    icmp_iph->ip_tos = 0;
    icmp_iph->ip_len = htons(icmp_m.len - sizeof(struct ether_header));
    icmp_iph->ip_id = htons(321);
    icmp_iph->ip_off = htons(0);
    icmp_iph->ip_ttl = 64;
    icmp_iph->ip_p = IPPROTO_ICMP;
	icmp_iph->ip_dst.s_addr = iph->ip_src.s_addr;
	icmp_iph->ip_src.s_addr = src_ip;
    icmp_iph->ip_sum = 0;

	//ICMP header
	icmph->icmp_type = type;
    icmph->icmp_code = code;
    icmph->icmp_id = 123;
    icmph->icmp_seq = 0;
	memcpy(&icmph->icmp_ip, iph, 8);
	icmph->icmp_cksum = 0;

	//Checksums and send
	icmph->icmp_cksum = icmp_checksum((uint16_t *)icmph, sizeof(struct icmp));
	icmp_iph->ip_sum = ip_checksum((uint8_t *)icmp_iph, sizeof(struct ip));
	send_packet(&icmp_m);
	fprintf(stdout, "sent ICMP %d %d\n", type, code);
}

/*---------------------------------------------------------------------------------------------------------------*/

/*forwards a packet*/
void forward(packet *m) {
	struct ether_header *eth = (struct ether_header *) m->payload;
	struct iphdr *iph = ((void *) eth) + sizeof(struct ether_header);

	uint8_t src_mac[6];
	get_interface_mac(m->interface, src_mac);
	struct in_addr ip;
	inet_aton(get_interface_ip(m->interface), &ip);
	uint32_t src_ip = (uint32_t)ip.s_addr;

	uint8_t brd_mac[6];
	for (int i = 0; i < 6; i++) brd_mac[i] = 255;

	//Check checkusm
	if (ip_checksum((void *) iph, sizeof(struct iphdr)) != 0) {
		return;
	}

	//Check ttl
	if (iph->ttl <= 1) {
		//*ICMP time exceded
		generate_ICMP_packet(m, 11, 0, src_mac, src_ip);
		return;
	}

	//Get destination ip
	uint32_t dest_ip = iph->daddr;

	//Get next hop ip
	struct route_table_entry *route = lpmatch(dest_ip);
	if (route == NULL) {
		//*ICMP unreachable
		generate_ICMP_packet(m, 3, 0, src_mac, src_ip);
		return;
	}

	//Get next hop mac
	uint8_t *mac = calloc(6, sizeof(uint8_t));
	int x = get_mac_from_ip(route->next_hop, mac); //*Search in cache 
	if (x) {
		//*Enq
		packet *queue_m = malloc(sizeof(packet));
		memcpy(queue_m, m, sizeof(packet));
		queue_enq(unsend_packets, queue_m);
		//*ARP request
		if (x == 1) {
			generate_ARP_packet(m, route->interface, brd_mac, route->next_hop, ARPOP_REQUEST);
		}
		return;
	}

	//Update checksum and ttl
	iph->ttl--;
	iph->check = 0;
	iph->check = ip_checksum((void *) iph, sizeof(struct iphdr));

	//Update destination mac and source mac in ether header
	memcpy(eth->ether_dhost, mac, 6);
	get_interface_mac(route->interface, eth->ether_shost);

	//Update interface and send packet
	m->interface = route->interface;
	send_packet(m);
	fprintf(stdout, "forwarded IP/ICMP packet\n");
}

/*****-------------------------------------------------------------------------------------------------------*****/

int main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	packet m;
	int rc;

	// Do not modify this line
	init(argc - 2, argv + 2);

	//1. Read and sort Route Table
	rtable = malloc(sizeof(struct route_table_entry) * 100000);
	DIE(rtable == NULL, "memory");
	rtable_len = read_rtable(argv[1], rtable);
	qsort(rtable, rtable_len, sizeof(struct route_table_entry), comp_rtable_desc);

	//2. Initialize Cache and Packet Queue
	cache_size = 0;
	unsend_packets = queue_create();
	queue unsend_packets_reserve;
	queue exchange;
	packet *p;
	unsend_packets_reserve = queue_create();

	//3. Other
	struct ether_header *eth;
	struct ip *iph;
	struct arp_header *arph;
	uint8_t mac[6];
	struct in_addr ip;
	uint8_t type;
	uint8_t code;
	int x;

/*---------------------------------------------------------------------------------------------------------------*/

	while (1) {
		fprintf(stdout, "*Waiting for packet...*\n");
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");
		/* TODO */

		fprintf(stdout, "*Got packet*\n");
		//Get ether header & interface mac and ips
		eth = (struct ether_header *) m.payload;
		inet_aton(get_interface_ip(m.interface), &ip);
		get_interface_mac(m.interface, mac);

/*---------------------------------------------------------------------------------------------------------------*/

		if (ntohs(eth->ether_type) == ETHERTYPE_IP) {

			//Get ip header
			iph = ((void *) eth) + sizeof(struct ether_header);

			if (iph->ip_p == IPPROTO_IP) {

				fprintf(stdout, "IP:\n");
				if (iph->ip_dst.s_addr == (uint32_t)ip.s_addr) {
					fprintf(stdout, "for me\n");
					continue;
				}
				fprintf(stdout, "forward\n");
				forward(&m);
			} else if (iph->ip_p == IPPROTO_ICMP) {

				//Get icmp type and code
				type = *(uint8_t *)(m.payload + 34);
				code = *(uint8_t *)(m.payload + 35);

				fprintf(stdout, "ICMP:\n");
				if (iph->ip_dst.s_addr == (uint32_t)ip.s_addr) {

					//*ICMP echo
					if (type == 8 && code == 0) {
						fprintf(stdout, "echo request\n");
						generate_ICMP_packet(&m, 0, 0, mac, (uint32_t)ip.s_addr);
					}
				} else if (code == 0) {
				
					//*ICMP forward
					fprintf(stdout, "forward\n");
					forward(&m);
				}
			}

/*---------------------------------------------------------------------------------------------------------------*/

		} else if (ntohs(eth->ether_type) == ETHERTYPE_ARP) {

			fprintf(stdout, "ARP:\n");
			//Get ARP header
			arph = ((void *) eth) + sizeof(struct ether_header);

			if (ntohs(arph->op) == ARPOP_REQUEST && arph->tpa == (uint32_t)ip.s_addr) {
				
				//*ARP reply with mac
				fprintf(stdout, "arp request\n");
				generate_ARP_packet(&m, m.interface, eth->ether_shost, arph->spa, ARPOP_REPLY);
			} else if (ntohs(arph->op) == ARPOP_REPLY) {
				
				//*Add to cache
				fprintf(stdout, "arp reply\n");
				x = 1;
				for (int i = 0; i < cache_size; i++) {
					if (cache[i].ip == arph->spa) {
						memcpy(cache[i].mac, arph->sha, 6);
						x = 0;
					}
				}
				if (x) {
					cache[cache_size].ip = arph->spa;
					memcpy(cache[cache_size].mac, arph->sha, 6);
					cache_size++;
				}

				//*Finish packet
				while (!queue_empty(unsend_packets)) {
					p = (packet *)queue_deq(unsend_packets);
					iph = ((void *) p->payload) + sizeof(struct ether_header);
					struct route_table_entry *route = lpmatch(iph->ip_dst.s_addr);
					if (route->next_hop == arph->spa) {
						forward(p);
					} else {
						queue_enq(unsend_packets_reserve, p);
					}
				}
				exchange = unsend_packets;
				unsend_packets = unsend_packets_reserve;
				unsend_packets_reserve = exchange;
			}
		}
	}
}