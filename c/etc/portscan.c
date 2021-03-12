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
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void usage(char *progname){
	printf("\nUsage:\n\t%s <dir_ip o nombre> <minport> <maxport>\n\n", progname);
}


int main(int argc, char** argv) {

	char *remoto;
	int max_port=1024;
	int min_port=0;
	int port;
	struct sockaddr_in direccion;
	int sockid;
	int conn;
	struct in_addr remoto_dir;
	char *error;
	struct hostent *host_dir;
	struct servent *servicio;

	if(argc != 4){
		usage(*argv);
		exit(1);
	}

	remoto = *(argv+1);
	min_port = atoi(*(argv+2));
	max_port = atoi(*(argv+3));

	//almacenamos la direccion en formato network:
	direccion.sin_family=AF_INET;
//	inet_aton(remoto, &remoto_dir);
//	direccion.sin_addr = remoto_dir;
	host_dir = gethostbyname(remoto);
	// el miembro h_addr de host_dir tiene la direccion
	// el miembro h_lenght tiene la longitud de la direccion
	memcpy((char *)&direccion.sin_addr, (char *)host_dir->h_addr, host_dir->h_length);

//	printf("Scanning: %s (%s)\n",remoto, inet_ntoa(direccion->sin_addr)); PENDIENTE


	for(port=min_port; port<=max_port;port++){
	//	printf("Analizando puerto %d...\n",port);
		if((sockid=socket(PF_INET, SOCK_STREAM,0))<0){
			error = "socket";
			goto err;
		}

		//almacenamos el puerto:
		direccion.sin_port = htons(port);

		conn=connect(sockid, (struct sockaddr *)&direccion, sizeof(direccion));

		close(sockid);

		if(!conn){
			servicio = getservbyport(htons(port),"tcp");
			printf("Puerto %d (%s) abierto!!\n",port, servicio->s_name);
		}
		

	}

	return 0;

err:
	fprintf(stderr,"%d (%s) - %s\n",errno, error, strerror(errno));
	exit(2);
}

/*

AF_UNIX			archivos tipo socket
AF_INET			estructuras de dato de conexion
	SOCK_DGRAM	socket datagrama	UDP
	SOCK_STREAM	socket stream		TCP


           struct hostent {
               char  *h_name;            * official name of host *
               char **h_aliases;         * alias list *
               int    h_addrtype;        * host address type *
               int    h_length;          * length of address *
               char **h_addr_list;       * list of addresses *
           }
           #define h_addr h_addr_list[0] * for backward compatibility *

       The members of the hostent structure are:

       h_name The official name of the host.

       h_aliases
              An array of alternative names for the host, terminated by a null pointer.

       h_addrtype
              The type of address; always AF_INET or AF_INET6 at present.

       h_length
              The length of the address in bytes.

       h_addr_list
              An  array of pointers to network addresses for the host (in network byte order), ter‐
              minated by a null pointer.

       h_addr The first address in h_addr_list for backward compatibility.

*/
