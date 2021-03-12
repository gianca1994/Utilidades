/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : JuncoTIC / UM               *
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
#include<netinet/in.h>

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
	direccion.sin_port=htons(1234);
	direccion.sin_addr.s_addr=inet_addr("127.0.0.1");


	//establecemos la conexion con el server (syn, syn-ack, ack) 
	if(connect(sockid, (struct sockaddr *)&direccion, sizeof(direccion))<0){
		error="connect";
		goto err;
	}

	printf("Conexion establecida!! Procedemos a enviar datos...\n");
	sleep(5);


/*

       #include <sys/types.h>
       #include <sys/socket.h>
      int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	  	sockfd: socket id
		addr: dir del servidor
		addrlen: long de la dir del servidor

*/

	// sockid está asociado al conn_sock del servidor, envio de datos

	count=sprintf(buffer,"hola mundo");

	if((send(sockid, buffer, count, 0))<0){
		error="send";
		goto err;
	}
	printf("Dato enviado... procedemos a cerrar la conexion...\n");
	sleep(20);
	

	close(sockid);

	return 0;

err:
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);

}

/*

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};



tcpip conexiones tcp:
	client	server
		> syn		\
		< ack +syn	|	establecimiento de la conexion -> conn_sock
		> ack		/
		...
		trafico cliente-servidor usando el conn_sock
		...
		< fin		\
		> ack + fin	|	fin de la conexion (circuito virtual tcp)
		< ack		/



*/