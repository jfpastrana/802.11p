#include <stdio.h>
#include <pcap.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 6

struct packet
{
	char mac_destination[20];
	char mac_source[20];
	char type[20];
	char lifetime;
	char traffic_class [20];
	char flags[20];
	char time_stamp[20];
	char latitude[20];
	char longitude[20];
	char speed[20];
} geonetworking;

int main()
{
	const char path[60]="/home/wlanp/scripts/telegram/toSend/packet.txt";
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0, j=0, len=0;
	pcap_t *adhandle;
	int res;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct tm *ltime;
	char timestr[16];
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	time_t local_tv_sec;
	FILE* file;
	char * ptr = NULL, * ptr2 = NULL;
	unsigned char packet[100];
    	unsigned char aux[100];
	unsigned int  dec;	
	
//	Retrieve the device list on the local machine
	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
//	Print the list 
	for(d=alldevs; d; d=d->next) {
        	printf("%d. %s", ++i, d->name);
        	if (d->description)
            		printf(" (%s)\n", d->description);
        	else
            		printf(" (No description available)\n");
    	}
	
    	if(i==0) {
        	printf("\nNo interfaces found!.\n");
        	return -1;
    	}
    
    	printf("Enter the interface number (1-%d):",i);
	inum=2;
    
    	if(inum < 1 || inum > i) {
        	printf("\nInterface number out of range.\n");
//        	Free the device list 
        	pcap_freealldevs(alldevs);
        	return -1;
    	}

	/* Jump to the selected adapter */

	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
    
	/* Open the device */
	if ( (adhandle= pcap_open_live(d->name, 100, 1, 5000, errbuf) ) == NULL) {
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported \n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
    
	printf("\nlistening on %s...\n", d->description);
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);
	/* Retrieve the packets */
	ptr=packet;
	pcap_next_ex( adhandle, &header, &pkt_data);        
	for (i=1; (i < header->caplen + 1 ); i++) {
                packet[i-1] = pkt_data[i-1];
		if ( (i % LINE_LEN) == 0) {
			printf("\n");
		}
        }
	len = sizeof(packet);
	if(packet[len-1]=='\n')
        	packet[--len] = '\0';
    	for(i = 1; i<len+1; i++) {
        	sprintf(aux+(i-1)*2, "%.2x", packet[i-1]);
	        if ( (i % LINE_LEN) == 0) {
        	        printf("\n");
                }

    	}
	file = fopen(path,"w");

	ptr = aux;
	memmove(geonetworking.mac_destination,ptr,12);
	memmove(geonetworking.mac_source,ptr+12,12);
	memmove(geonetworking.type,ptr+24,4);
	memmove(geonetworking.traffic_class,ptr+42,2);
	memmove(geonetworking.flags, ptr+44,2);
	memmove(geonetworking.time_stamp,ptr+70,7);
	memmove(geonetworking.latitude,ptr+78,8);
	memmove(geonetworking.longitude,ptr+86,8);
	memmove(geonetworking.speed,ptr+94,4);	

	printf("\n%s",geonetworking.mac_destination);
        printf("\n%s",geonetworking.mac_source);
        printf("\n%s",geonetworking.type);
        printf("\n%s",geonetworking.traffic_class);
        printf("\n%s",geonetworking.flags);
        printf("\n%s",geonetworking.time_stamp);
        printf("\n%s",geonetworking.latitude);
        printf("\n%s",geonetworking.longitude);
        printf("\n%s",geonetworking.speed);



        file = fopen(path,"w");
	fputs("\nMAC Destination: ",file);
	fprintf(file,"%s",geonetworking.mac_destination);
        
	fputs("\nMAC Source: ",file);
        fprintf(file,"%s",geonetworking.mac_source);
        
	fputs("\nType: ",file);
	fprintf(file,"%s",geonetworking.type);
        
	fputs("\nTraffic class: ",file);
        fprintf(file,"%s",geonetworking.traffic_class);
        
	fputs("\nFlags: ",file);
        fprintf(file,"%s",geonetworking.flags);
        
	fputs("\nTime stamp: ",file);
        sscanf(geonetworking.time_stamp, "%x", &dec);
        fprintf(file,"%d",dec);
        
	fputs("\nLatitude: ",file);
        fprintf(file,"%s",geonetworking.latitude);

	fputs("\nLongitude: ",file);
        fprintf(file,"%s",geonetworking.longitude);

        fputs("\nSpeed: ",file);
	sscanf(geonetworking.speed,"%x", &dec);
        fprintf(file,"%d",dec);
	
	fclose(file);		

	if(res == -1){
		printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
		return -1;
	}
	
    return 0;
}
