/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com / um.edu.ar    *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

// cabeceras de pcap y relacionadas
#include<pcap.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<netinet/ip_icmp.h>
#include<netinet/ether.h>
#include<net/ethernet.h>
#include<arpa/inet.h>
#include<sys/socket.h>


/*
	Sniffer de trafico de red
	software que lee tramas de red de la interfaz de red

	Modelo tcp/ip
		aplicacion		http, ftp, dns, ssh, ...	-
		transporte		tcp(6), udp(17), icmp(1)	segmento
		red				ip							paquete
		enlace			ethernet					trama

		[hr eth][hr ip][hr tcp/udp][dato]
                       ^


	Biblioteca: Packet capture - pcap

	Procedimiento:
		1) obtener un manejador o handler de un dispositivo de red
			pcap_open_live()

		2) "ciclar" sobre los paquetes capturados
			pcap_loop()
			requiere una funcion handler definida
			
	Instalacion de la biblioteca pcap:
		aptitude install libpcap-dev


	Notas:
		/etc/protocols

	Compilamos con: gcc archivo.c -lpcap


*/

//supongamos que la llamada es
//	./sniffer <iface>


//prototipos de funciones

void procesar_pkg(u_char *user, const struct pcap_pkthdr *h,const u_char *bytes);
void mostrar_eth(const u_char *bytes, int size);
void mostrar_ip(const u_char *bytes, int size);
void mostrar_tcp(const u_char *bytes, int size);

int main(int argc, char** argv) {

	char *interfaz;

	interfaz = *(argv+1);
	pcap_t *handler;

	/* abrimos el dispositivo de red: pcap_open_live()
		pcap_t *pcap_open_live(const char *device, int snaplen,
               int promisc, int to_ms, char *errbuf);
		
			device: dispositivo de red
			snaplen: cantidad de bytes a leer (max 65536), trunca si la trama es mas grande
			promisc: 1:modo promiscuo, 0:modo no promiscuo
			to_ms: milisegundos de delay entre capturas (0: captura todo el trafico)
			errbuf: buffer donde guardar codigos de errores y warnings
	*/

	handler = pcap_open_live(interfaz, 65536, 0, 0, NULL);

	/* empezamos a capturar: pcap_loop()

       int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user);
		*p			dispositivo
		cnt			cantidad de paquetes a procesar (-1: no se detiene)
		callback	funcion a llamar con cada trama
		*user		puntero char para pasar info


       typedef void (*pcap_handler)(u_char *user, const struct pcap_pkthdr *h,const u_char *bytes);
	   	user		parametro pasado opcional
		*h			header de paquete pcap
		*bytes		puntero a la trama completa
	*/


	pcap_loop(handler, -1, procesar_pkg, NULL);


	return 0;
}


void procesar_pkg(u_char *user, const struct pcap_pkthdr *h,const u_char *bytes){

	int size = h->len;		//longitud de la trama
	struct iphdr *ip_hdr;

	ip_hdr = (struct iphdr*)(bytes+sizeof(struct ethhdr));


	switch(ip_hdr->protocol){
	case 1:
//		mostrar_icmp(bytes, size);
		break;
	case 6:
	printf("-----------------------------------------------------------\n");
	mostrar_eth(bytes, size);
	mostrar_ip(bytes, size);
		mostrar_tcp(bytes, size);
		break;
	case 17:
//		mostrar_udp(bytes, size);
		break;
	}


}


void mostrar_eth(const u_char *bytes, int size){
	struct ethhdr *eth_hdr = (struct ethhdr*)bytes;

	printf("Mac destino:\t");

	int i;

	for(i=0;i<6;i++)
		printf("%.2X:", eth_hdr->h_dest[i]);

	printf("\tMac origen:\t");
	for(i=0;i<6;i++)
		printf("%.2X:", eth_hdr->h_source[i]);

	printf("\tEth_type: %d\n",eth_hdr->h_proto);

}


void mostrar_ip(const u_char *bytes, int size){
	struct iphdr *ip_hdr;
	struct sockaddr_in ip_src, ip_dst;

	char proto[5];

	ip_hdr = (struct iphdr*)(bytes+sizeof(struct ethhdr));

	ip_src.sin_addr.s_addr = ip_hdr->saddr;
	ip_dst.sin_addr.s_addr = ip_hdr->daddr;

	printf("IP Origen: %s",inet_ntoa(ip_src.sin_addr));
	printf("\tIP Destino: %s",inet_ntoa(ip_dst.sin_addr));

	switch(ip_hdr->protocol){
	case 1:
		strcpy(proto, "icmp");
		break;
	case 6:
		strcpy(proto, "tcp");
		break;
	case 17:
		strcpy(proto, "udp");
		break;
	}

	printf("\tId: %d, Proto: %d(%s)\n", ip_hdr->id, ip_hdr->protocol,proto);

}


void mostrar_tcp(const u_char *bytes, int size){
	struct tcphdr *tcp_hdr;
	struct iphdr *ip_hdr;
	int tamanio_iphdr;

	ip_hdr = (struct iphdr*)(bytes+sizeof(struct ethhdr));

	tamanio_iphdr = ip_hdr->ihl * 4;

	tcp_hdr = (struct tcphdr*)(bytes+sizeof(struct ethhdr)+tamanio_iphdr);

	printf("Puerto origen: %d", ntohs(tcp_hdr->source));
	printf("\tPuerto destino: %d", ntohs(tcp_hdr->dest));

	printf("\n");

}




/*

CABECERAS Y ESTRUCTURAS DE DATOS:



struct ethhdr{
	uint8_t h_dest[ETH_ALEN];			mac dest
	uint8_t h_source[ETH_ALEN];			mac orig
	uint16_t h_proto;					proto de capa 3 (0x0800)
}


HEADER IP

    0                   1                   2                   3   
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   payload iP
   

struct iphdr
  {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
	    unsigned int ihl:4;					internet header len
	    unsigned int version:4;				version
	#elif __BYTE_ORDER == __BIG_ENDIAN
	    unsigned int version:4;
	    unsigned int ihl:4;
	#else
	# error "Please fix <bits/endian.h>"
	#endif
	    u_int8_t tos;
	    u_int16_t tot_len;
	    u_int16_t id;
	    u_int16_t frag_off;
	    u_int8_t ttl;
	    u_int8_t protocol;
	    u_int16_t check;
	    u_int32_t saddr;
	    u_int32_t daddr;
	   //The options start here.
  };

ICMP
struct icmphdr
{
  u_int8_t type;        * message type *
  u_int8_t code;        * type sub-code *
  u_int16_t checksum;
  union {
    struct{
      u_int16_t id;
      u_int16_t sequence;
    } echo;         * echo datagram *
    u_int32_t   gateway;    * gateway address *
    struct{
      u_int16_t __unused;
      u_int16_t mtu;
    } frag;         * path mtu discovery *
  } un;
};


HEADER TCP:

0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data | Re- | ECN |U|A|P|R|S|F|                               |
| Offset| ser-|-----|R|C|S|S|Y|I|            Window             |
|       | ved |N|C|E|G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |         Urgent Pointer        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

struct tcphdr
  {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
#  elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};



HEADER UDP:
	0                                  31  
	+--------+--------+--------+--------+ 
	|     Source      |   Destination   | 
	|      Port       |      Port       | 
	+--------+--------+--------+--------+ 
	|                 |                 | 
	|     Length      |    Checksum     | 
	+--------+--------+--------+--------+ 
	|                                   |
	/          data octets ...          /
	|                                   |
	+-----------------------------------+


struct udphdr
{
  u_int16_t uh_sport;       * source port *
  u_int16_t uh_dport;       * destination port *
  u_int16_t uh_ulen;        * udp length *
  u_int16_t uh_sum;     * udp checksum *
};




*/



