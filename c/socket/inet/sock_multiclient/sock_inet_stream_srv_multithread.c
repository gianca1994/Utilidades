/*******************************************
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com  / um.edu.ar   *
*******************************************/

/*
	Uso:
		./servidor <puerto>
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
#include<pthread.h>

#define BUF_SIZE 1024

void* sock_handler(void* dato);

int main(int argc, char** argv) {

	char *error;

	int sockid, conn_sock, count;
	struct sockaddr_in direccion;
	char buffer[BUF_SIZE];
	int pid;
	pthread_t thread_id;


	//creamos el socket inet-stream
	if((sockid=socket(PF_INET,SOCK_STREAM,0))<0){
		error="socket";
		goto err;
	}


	//seteamos la direccion en la que va a escuchar
	direccion.sin_family=AF_INET; //address family
	direccion.sin_port=htons(atoi(*(argv+1)));	//atoi ascii to integer
	direccion.sin_addr.s_addr=htonl(INADDR_ANY); //0.0.0.0


	//asociamos el socket con la direccion (bind)
	if((bind(sockid, (struct sockaddr *)&direccion, sizeof(direccion)))<0){
		error="bind";
		goto err;
	}


	// seteamos la cantidad de conexiones concurrentes en cola
	listen(sockid,1);


	//dejamos escuchando al proceso en el socket ip:puerto
	while(conn_sock=accept(sockid,NULL,NULL)){
		if (conn_sock<0){
			error="accept";
			goto err;
		}

		if(pthread_create(&thread_id,NULL,sock_handler, (void*)&conn_sock)){
			error="pthread_create";
			goto err;
		}

		printf("Conexion delegada al hilo %li\n",thread_id);
	}



	// ya tenemos un conn_sock de datos asociado con el cliente conectado
	return 0;

err:
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);

}


void* sock_handler(void* dato){
	char buffer[BUF_SIZE];
	int conn_sock_th, count;
	pthread_detach(pthread_self());

	conn_sock_th = *((int*)dato);

	printf("Iniciando hilo %li\n",pthread_self());

	while(count=recv(conn_sock_th,buffer,BUF_SIZE,0)){
		*(buffer+count)='\0';
		printf("Recibiendo datos (%li): %s\n",pthread_self(),buffer);
	}
	close(conn_sock_th);
	pthread_exit(NULL);

}