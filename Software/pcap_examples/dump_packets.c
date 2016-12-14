/**********************************************************************
* file: dump_packets.c
* date: 25-Abril-2005
* Author: Alejandro Lopez Monge
*
* Compilacion: gcc -lpcap -o dump_packets dump_packets.c
*
* Este ejemplo muestra como capturar paquetes y guardarlos en un fichero
* para su posterior procesamiento, todo ello empleando las funciones propias
* de la API de Libpcap. Combinamos varias de las cosas que ya hemos aprendido 10
* aplicar filtros, este programa solo captura los paquetes que sean de la misma
* red y que vayan destinados al puerto 23, y una cosa nueva las estadisticas.
**********************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define IFSZ 16
#define FLTRSZ 120
#define MAXHOSTSZ 256

int usage(char *progname)
{
	printf("Uso: %s <interfaz> [<fichero salida>]\n", basename(progname));
	return -1;
}

int main()
{
	pcap_t *fp;
	pcap_if_t *alldevs, *d;
	//struct pcap_stat ps; /* estadisticas */
	pcap_dumper_t *pd; /* dump file */
	char ifname[IFSZ]; /* nombre de la interfaz */
	char filename[80] = "savefile3.pcap"; /* nombre del dump file*/
	char errbuf[PCAP_ERRBUF_SIZE]; /* descripcion del error */
	char lhost[MAXHOSTSZ]; /* nombre del localhost */
	char fltstr[FLTRSZ]; /* texto del bpf filter*/
	char prestr[80]; /* prefijo para los errores */
	struct bpf_program prog; /* BPF compilado */
	int optimize = 1;
	int snaplen = 100; /* Tama˜no por paquete */
	int promisc = 1; /* modo de captura */
	int to_ms = 20; /* timeout*/
	int count = 10; /* numero de paquetes a capturar */
	int net = 0; /* direccion de red*/
	int mask = 0; /* mascara de subred */
	char netstr[INET_ADDRSTRLEN]; /* direccion de red en modo texto*/
	char maskstr[INET_ADDRSTRLEN]; /* mascara de red en modo texto */
	int linktype = 0; /* tipo de enlace de datos*/
	int pcount = 0; /* Numero de paquetes leidos */
	unsigned int inum, i=0;	

        //dev = pcap_lookupdev(errbuf);
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
                printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
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
	
	if (!(fp = pcap_open_live(d->name, snaplen, promisc, to_ms, errbuf))) {
		fprintf(stderr, "Error al abrir la interfaz %s: %s\n",ifname, errbuf);
		return -1;
	}
	/* Comenzamos el DUMP */ 
	if ((pd = pcap_dump_open(fp,filename)) == NULL) {
		fprintf(stderr,"Error abriendo el fichero \" %s\" para escritura: %s\n",filename, pcap_geterr(fp));
		return -1;
	}
	
	if ((pcount = pcap_loop(fp, count, &pcap_dump, (char *)pd)) < 0) {
		pcap_perror(fp,prestr);
		return -1;
	}
	if (!(linktype = pcap_datalink(fp))) {
		fprintf(stderr,"Error obteniendo el data link %s",ifname);
		return -1;
	}
	pcap_dump_close(pd);
	pcap_close(fp);
}

