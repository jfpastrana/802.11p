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
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

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
        int j=0,i=0, k=0;
        void* buffer = (void*)malloc(BUF_SIZ);
        unsigned char* data = buffer + 14;
        srand(time(NULL));
	unsigned int previous_channel = 0;
	unsigned int next_channel = 0;
        struct timeval ti, tf;
        double tiempo, t_total;

        /* Restarting channel frequency and setting OCB interface	*
	* in Control channel. Fixing the bitrate at 6Mbit/s		*/
	
        system("iw dev ocb0 ocb leave");
        system("iw dev ocb0 ocb join 5900 10MHZ");
	system("iw dev ocb0 set bitrates legacy-5 12");
	system("iw dev reg set AA");

        /* Get interface name */
        if (argc > 1) {
                strcpy(ifName, argv[1]);
	}
        else {
                strcpy(ifName, DEFAULT_IF);
	}
        /* Open RAW socket to send on */
        if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
            perror("Error opening the RAW socket");
        }
        /* Get the index of the interface to send on */
        memset(&if_idx, 0, sizeof(struct ifreq));
        strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
        if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
            perror("Error with the index of the interface");
	}
        /* Get the MAC address of the interface to send on */
        memset(&if_mac, 0, sizeof(struct ifreq));
        strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
        if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
            perror("Error getting the MAC address");
	}

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


	do {
		tx_len=0, j=0, k=0;
		/* Ethertype field */
        	eh->ether_type = htons(ETH_P_IP);
        	tx_len += sizeof(struct ether_header);

        	/* Packet data */
		/* Channel information */
		previous_channel = next_channel;
		//printf("\nDeciding next channel...");
		next_channel = rand()%7;
                if ( i < 1) {
                       // printf("\nEstado inicial - A");
                        //printf("\nEnvar trama con channel = 0");
	                next_channel = 0;
                }
                if((previous_channel != next_channel && previous_channel > 0 && next_channel > 0)) {
/*                	if (previous_channel==1 && next_channel ==2) {i
				printf("\nAllowed comnmutation");
			}
			else if (previous_channel == 3 && next_channel == 4) {
				printf("\nallowed conmutation");
			}
			else next_channel = 0;
*/
			next_channel = 0;    
	        }
                
		data[j] = next_channel + '0';
		sendbuf[tx_len++] = data[j];
		//printf("\nThe next channel will be: %d",next_channel);
		sendbuf[tx_len++] = i + '0';
		/*fill the frame with some data*/
/*
        	for (j = 1; j < 1500; j++) {
                	data[j] = (unsigned char)((int) (255.0*rand()/(RAND_MAX+1.0)));
                	sendbuf[tx_len++] = data [j];
        	}
*/

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
        	
		for(k=0;k<2;k++) {
			/* Send packet */
        		//printf("\nSend the packet: %d",k);
			if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
            			printf("Send failed\n");
			}
			system("sleep 0.1");
		}
                gettimeofday(&tf, NULL);   // Instante final
                if(tiempo>0) {
			tiempo= (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
			t_total = t_total + tiempo;
		}
		
                gettimeofday(&ti, NULL);   // Instante inicial
	
		if(next_channel != previous_channel) {
	                //printf("\nLeaving the channel");
        	        system("iw dev ocb0 ocb leave");
			switch (next_channel) {
				case 0:		system("iw dev ocb0 ocb join 5900 10MHZ");
						break;
                       		case 1:		system("iw dev ocb0 ocb join 5880 10MHZ");
						break;
	                	case 2:		system("iw dev ocb0 ocb join 5890 10MHZ");
						break;
               			case 3:       	system("iw dev ocb0 ocb join 5870 10MHZ");
						break;
                       		case 4:       	system("iw dev ocb0 ocb join 5860 10MHZ");
						break;
                       		case 5:       	system("iw dev ocb0 ocb join 5910 10MHZ");
						break;
                       		case 6:       	system("iw dev ocb0 ocb join 5920 10MHZ");
						break;
				default:	printf("\nWorng channel");
			}
			system("sleep 5");
		}
		//printf("\nJoined to channel: %d",next_channel);
		i++;
	}while (i<100);
	printf("\n\nThe average time between messages was: %f\n\n", t_total);

        return 0;
}   
