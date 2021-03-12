/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com                *
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
#define SOCK_PATH "/tmp/socket1"

int main(int argc, char** argv) {

	char *error;
	int sockid;		//id del socket
	struct sockaddr_un direccion;
	char mensaje[MSG_SIZE];
	int count;

	if((sockid=socket(PF_UNIX,SOCK_DGRAM,0))<0){
		error="socket";
		goto err;
	}
	/*
	   int socket(int domain, int type, int protocol);
	   	domain: familia de protocolo (PF_)
		type: tipo de socket (DGRAM o STREAM)
		protocol: 0 (unix es un archivo)

	*/

	//cargamos la direccion:
	direccion.sun_family=AF_UNIX;
	strcpy(direccion.sun_path,SOCK_PATH);

	/*
	Asociamos la direccion al receptor, "dejar escuchando"
	bind
	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
			id del socket
			direccion creada
			long de la direccion creada
	*/
	if((bind(sockid,(struct sockaddr *)&direccion, sizeof(direccion)))<0){
		error="bind";
		goto err;
	}
	//se crea el archivo del socket


	/*
	recibimos el dato
		ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
		sockid		id del socket
		buf			dir de memoria donde guardar el mensaje
		len			long max del mensaje
		flags		opcionales, 0 por default
		src_addr	direccion origen, opcional
		addrlen		long de la direccion del origen
	*/

//	if((count=recvfrom(sockid,mensaje,100,0,NULL,NULL))<0){
	if((count=recv(sockid,mensaje,MSG_SIZE,0))<0){
		error="recvfrom";
		goto err;
	}
	*(mensaje+count)='\0';
	printf("Mensaje leido: %s\n",mensaje);

	close(sockid);
	return 0;

err:
	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
	exit(1);

	return 0;
}

/*

Familias de sockets (protocol family)
	PF_UNIX	locales		archivo tipo socket
	PF_INET	red tcp/ip	direccion:puerto

Direccion del socket: (address family)
	AF_UNIX
	AF_INET
		SOCK_DGRAM		-> udp
			no confiable
			el emisor envia, el receptor recibe
		SOCK_STREAM		-> tcp
			confiabilidad
			el server espera conexiones
			cdo un cliente se conecta, el server crea un sock de datos(fd)
			el server y cliente interactuan con ese sock de datos

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