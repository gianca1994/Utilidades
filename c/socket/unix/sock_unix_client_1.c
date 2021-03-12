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
#define SOCK_PATH "/tmp/socket1"

//emisor

int main(int argc, char** argv) {

	char *error;
	int sockid;		//id del socket
	struct sockaddr_un direccion;
	char mensaje[MSG_SIZE] = "hola mundo";

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

	//enviamos datos al socket:
	if((sendto(sockid,*(argv+1),strlen(*(argv+1)),0,(struct sockaddr *)&direccion,sizeof(direccion)))<0){
		error="sendto";
		goto err;
	}

	printf("Enviado...\n");

	/*
	ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
		sockfd		socket id o file-descriptor del socket Unix
		buf			buffer donde esta el mensaje
		len			longitud del mensaje a enviar
		flags		flags, 0 por default
		dest_addr	direccion del destinatario
		addrlen		long de la direccion
	*/

	close(sockid);
	return 0;

err:
	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
	exit(1);

	return 0;
}