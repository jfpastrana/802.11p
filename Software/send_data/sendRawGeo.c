/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#define MY_DEST_MAC0    0xFF
#define MY_DEST_MAC1    0xFF
#define MY_DEST_MAC2    0xFF
#define MY_DEST_MAC3    0xFF
#define MY_DEST_MAC4    0xFF
#define MY_DEST_MAC5    0xFF

#define DEFAULT_IF      "ocb0"
#define BUF_SIZ         1518


int main(int argc, char *argv[])
{
        int sockfd;
        struct ifreq if_idx;
        struct ifreq if_mac;
        int tx_len = 0;
        char sendbuf[BUF_SIZ];
        struct ether_header *eh = (struct ether_header *) sendbuf;
        struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
        struct sockaddr_ll socket_address;
        char ifName[IFNAMSIZ];
        int j=0,i=0,k=0;
        void* buffer = (void*)malloc(BUF_SIZ);
        unsigned char* data = buffer + 14;
	unsigned char geonetworking[40] = {0x01,0x00,0xF1,0x01,0x00,0x10,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xE2,0xDC,0x99,0xFA,0x00,0x7B,0x9E,0xE9,0x14,0x00,0x1C,0x6D,0xD2,0x44,0x03,0xE5,0x68,0x22,0x00,0x00,0x00,0x00};

        /* Get interface name */
        if (argc > 1)
                strcpy(ifName, argv[1]);
        else
                strcpy(ifName, DEFAULT_IF);

        /* Open RAW socket to send on */
        if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
            perror("socket");
        }
        /* Get the index of the interface to send on */
        memset(&if_idx, 0, sizeof(struct ifreq));
        strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
        if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
            perror("SIOCGIFINDEX");
        /* Get the MAC address of the interface to send on */
        memset(&if_mac, 0, sizeof(struct ifreq));
        strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
        if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
            perror("SIOCGIFHWADDR");
	
	do{
		tx_len=0,
        	/* Construct the Ethernet header */
        	memset(sendbuf, 0, BUF_SIZ);
        	/* Ethernet header */
        	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
        	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
        	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
        	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
        	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
       	 	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
        	eh->ether_dhost[0] = MY_DEST_MAC0;
        	eh->ether_dhost[1] = MY_DEST_MAC1;
        	eh->ether_dhost[2] = MY_DEST_MAC2;
        	eh->ether_dhost[3] = MY_DEST_MAC3;
        	eh->ether_dhost[4] = MY_DEST_MAC4;
        	eh->ether_dhost[5] = MY_DEST_MAC5;


        	/* Ethertype field */
        	eh->ether_type = htons(ETH_P_IP);
        	tx_len += sizeof(struct ether_header);

        	/* Packet data - GeoNetworking*/
        	/*fill the frame with some data*/

		if (k==255) k=0;
        	for (j = 0; j < sizeof(geonetworking); j++) {
                	data[j] = geonetworking[j];
			data[23]=(unsigned char)((int) (k));
                	sendbuf[tx_len++] = data [j];
        	}
        	/* Index of the network device */
        	socket_address.sll_ifindex = if_idx.ifr_ifindex;
        	/* Address length*/
        	socket_address.sll_halen = ETH_ALEN;
        	/* Destination MAC */
        	socket_address.sll_addr[0] = MY_DEST_MAC0;
        	socket_address.sll_addr[1] = MY_DEST_MAC1;
        	socket_address.sll_addr[2] = MY_DEST_MAC2;
        	socket_address.sll_addr[3] = MY_DEST_MAC3;
     	   	socket_address.sll_addr[4] = MY_DEST_MAC4;
        	socket_address.sll_addr[5] = MY_DEST_MAC5;
       		/* Send packet */
        	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)	
			printf("Send failed\n");
		k++;
	}while (1);
        return 0;
}
   
