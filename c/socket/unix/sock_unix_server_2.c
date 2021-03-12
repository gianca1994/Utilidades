/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : Junco TIC                   *
* Contact    : juncotic.com                *
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
#include<sys/un.h>

#define MSG_SIZE 128

void usage(char* progname){
	printf("Usage:\n\t%s <pathname>\n\n",progname);
}


int main(int argc, char** argv) {

	char *error;
	int sockid;		//id del socket
	struct sockaddr_un direccion;
	char mensaje[MSG_SIZE];
	int count;

	if(argc<2){
		usage(*argv);
		exit(1);
	}

	if((sockid=socket(PF_UNIX,SOCK_DGRAM,0))<0){
		error="socket";
		goto err;
	}

	//cargamos la direccion:
	direccion.sun_family=AF_UNIX;
	strcpy(direccion.sun_path,*(argv+1));

	if((bind(sockid,(struct sockaddr *)&direccion, sizeof(direccion)))<0){
		error="bind";
		goto err;
	}
	//se crea el archivo del socket


	while(count=recv(sockid,mensaje,MSG_SIZE,0)){
		if(count<0){
			error="recvfrom";
			goto err;
		}
		printf("Mensaje leido: %s",mensaje);
		if(!strcmp(mensaje,"exit\n")){
			printf("Gracias por participar\n");
			break;
		}
	}

	close(sockid);
	unlink(*(argv+1));
	return 0;

err:
	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
	exit(1);

	return 0;
}

/*

Familias de sockets (protocol family)
	PF_UNIX
	PF_INET

Direccion del socket: (address family)
	AF_UNIX
	AF_INET
		SOCK_DGRAM		-> udp
		SOCK_STREAM		-> tcp

Sockets Unix:
	- procesos no relacionados
	- direccion: path
	- socket: archivo
	PF_UNIX

	- conceptos de sockets:
		proto de transporte
		puerto origen y destino
		direccion ip origen y destino
		--> socket INET / Internet / TCP/IP
		-> procesos en diferentes equipos
		PF_INET

Funciones:
	Emisor
		creamos el socket (socket)
		enviamos datos (sendto)
		la dir es de tipo "struct sockaddr_un"
	Receptor
		creamos el socket (socket)
		bindeamos o asociamos el socket a la direccion (bind)
		recibimos datos (recvfrom)


           struct sockaddr_un {
               sa_family_t sun_family;               AF_UNIX 
               char        sun_path[UNIX_PATH_MAX];  pathname 
           };


Trabajo con un socket:
	receptor
		creamos el struct sockaddr_un
		asignamos los datos de AF y path
		creamos el socket con "socket" -> estructura de datos
		asociamos la dir con el socket: "bind"
		dejamos al proc esperando datos: recvfrom
	
	emisor
		creamos el struct sockaddr_un
		asignamos AF y path
		creamos el socket con "socket"
		enviamos datos a la direccion con "sendto"

*/