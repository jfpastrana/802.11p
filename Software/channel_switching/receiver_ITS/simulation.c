#include <stdio.h>
#include <pcap.h>
#include <sys/time.h>

#define LINE_LEN 16


int main(int argc, char *argv[])
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	int numero, res;
    	pcap_if_t *alldevs, *d;
    	pcap_t *fp;
    	u_int inum, i=0;
    	long int sec_stamp=0, usec_stamp=0;
    	struct pcap_pkthdr *header;
    	const u_char *pkt_data;
	//unsigned char channel = 8;
        struct timeval ti, tf;
        double tiempo;
	int pcount=0;
        pcap_dumper_t *pd;
	int packets=1;
	unsigned  int next_channel=0, previous_channel=0;;
	int found = 0;
	int average=0;
	int t_total=0;

	/* Setting OCB0 interface in Control channel */
	system("iw dev ocb0 ocb leave");
	system("iw dev ocb0 ocb join 5900 10MHZ");
        /* Setting bitrate */
        system("iw dev ocb0 set bitrates legacy-5 12");
        
	/* Setting OCB interface in Control channel */
        system("iw dev ocb1 ocb leave");
        system("iw dev ocb1 ocb join 5880 10MHZ");
        /* Setting bitrate */
        system("iw dev ocb1 set bitrates legacy-5 12");
	
	if( pcap_findalldevs(&alldevs, errbuf) == -1) {
		fprintf(stderr, "Error in pcap_findallfevs: %s\n", errbuf);
		return -1;
	}
        /* Print the list */
        for(d=alldevs; d; d=d->next) {
		printf("%d. %s", ++i, d->name);
		if (d->description) {
                	printf(" (%s)\n", d->description);
            	}
		else {
                	printf(" (No description available)\n");
		}
        }
        if(i==0) {
		printf("\nNo interfaces found!.\n");
		return -1;
        }	
        printf("Enter the interface number (1-%d):",i);
        scanf("%d", &inum);
        
        if(inum < 1 || inum > i) {
		printf("\nInterface number out of range.\n");
            	/* Free the device list */
            	pcap_freealldevs(alldevs);
            	return -1;
        }
        
        /* Jump to the selected adapter */
        d=alldevs;
	for(i=0; i< inum-1 ; i++) {
		d=d->next;
	}
	printf("\nInterface switched");
        
        /* Open the device */
        if ( (fp= pcap_open_live(d->name, 100, 1, 20, errbuf) ) == NULL) {
            	fprintf(stderr,"\nError opening adapter\n");
            	return -1;
        }

	previous_channel = 0;
	/* Read the packets */
    	while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0) {
		//printf("\nEsperando paquete");
		if(res == 0)
            		/* Timeout elapsed */
            		continue;
	       	/* print pkt timestamp and pkt len */
        	//printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len); 			/* Print the packet */
        	//printf("Elapsed time since last message:%ld:%ld\n", header->ts.tv_sec - sec_stamp, header->ts.tv_usec - usec_stamp);  
		/*
		for (i=1; (i < header->caplen + 1 ) ; i++) {
            		printf("%.2x ", pkt_data[i-1]);
            		if ( (i % LINE_LEN) == 0) {
				printf("\n");
			        found = 1;
			}
        	}
		*/
//                printf("\nChannel: %c",pkt_data[14]);
                next_channel = pkt_data[14] - '0';
              	printf("\nEl numero del canal es: %d",next_channel);

                gettimeofday(&ti, NULL);   // Instante inicial
		if( next_channel != previous_channel) {

			system("iw dev ocb1 ocb leave");
			switch (next_channel) {
                        	case 0:		system("iw dev ocb1 ocb join 5880 10MHZ");
						break;
                        	case 1:		system("iw dev ocb1 ocb join 5880 10MHZ");
                                        	break;
                        	case 2:		system("iw dev ocb1 ocb join 5890 10MHZ");
                                        	break;
                        	case 3:		system("iw dev ocb1 ocb join 5870 10MHZ");
                                        	break;
                        	case 4:		system("iw dev ocb1 ocb join 5860 10MHZ");
                                        	break;
                        	case 5:		system("iw dev ocb1 ocb join 5850 10MHZ");
                                        	break;
                        	case 6:		system("iw dev ocb1 ocb join 5910 10MHZ");
                                        	break;
                        	default:	printf("\nWorng channel");
						system("iw dev ocb1 ocb join 5880 10MHZ");
                	}
		previous_channel = next_channel;
		}
                gettimeofday(&tf, NULL);   // Instante final
               	tiempo= (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
               	t_total= t_total + tiempo;
               	//printf("\nThe time elapsed to switch the channel was: %g ms\n", tiempo);
               	//printf("\nNumero de paquete: %d", packets++);
               	printf("\nTiempo medio de conmutacion de canal(ms): %d, numero de paquetes recogidos: %d", t_total/packets, packets);
		packets++;
	}
	if(res == -1){
        	printf("Error reading the packets: %s\n", pcap_geterr(fp));
        	return -1;
    	}
        pcap_dump_close(pd);
        pcap_close(fp);

	return(0);
}
