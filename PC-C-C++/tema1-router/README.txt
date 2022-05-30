Cristescu Diana 324CDa

Functions:

comp_rtable_desc = comparator function for sorting rtable (in descending order based on netmask)
route_table_entry = finds the first matching entry in rtable or NULL
get_mac_from_ip = gets an ip's mac by searching in cache
generate_ARP_packet = generates an arp packet
generate_ICMP_packet = generates an icmp packet
forward = forwards a packet


Main:

Test if packet is IP or ARP:
A. IP:
    1. ip:
        a. it's for me -> do nothing
        b. else -> forward
    2. icmp:
        a. it's for me & echo ping (request) -> ping reply
        b. else -> forward
B. ARP:
    1. request:
        a. generate arp reply
    2. reply:
        a. add mac to cache
        b. search queue for packets that match the ip from the arp packet and forward them