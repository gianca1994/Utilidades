/*******************************************
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com  / um.edu.ar   *
*******************************************/

/*
	Uso:
		./cliente ip_servidor puerto
*/


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
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(int argc, char** argv) {

	char *error;

	int sockid, conn_sock, count;
	struct sockaddr_in direccion;
	char buffer[BUF_SIZE];


	//creamos el socket inet-stream
	if((sockid=socket(PF_INET,SOCK_STREAM,0))<0){
		error="socket";
		goto err;
	}


	//seteamos la direccion en la que va a escuchar
	direccion.sin_family=AF_INET; //address family
	direccion.sin_port=htons(atoi(*(argv+2)));
	direccion.sin_addr.s_addr=inet_addr(*(argv+1));


	//establecemos la conexion con el server (syn, syn-ack, ack) 
	if(connect(sockid, (struct sockaddr *)&direccion, sizeof(direccion))<0){
		error="connect";
		goto err;
	}

	printf("Conexion establecida!! Soy %d\n",getpid());

	// sockid está asociado al conn_sock del servidor, envio de datos

	while(count=read(STDIN_FILENO,buffer,BUF_SIZE)){

		if((send(sockid, buffer, count, 0))<0){
			error="send";
			goto err;
		}
		printf("%d enviado...\n",getpid());
		if((recv(sockid, buffer, BUF_SIZE,0))<0){
			error="recv";
			goto err;
		}
		printf("%d recibiendo ACK: %s",getpid(),buffer);
	}

	close(sockid);

	return 0;

err:
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);

}