/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : JuncoTIC / UM               *
* Contact    : juncotic.com                *
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

#define BUF_SIZE 1024

int main(int argc, char** argv) {

	char *error;

	int sockid, conn_sock, count;
	struct sockaddr_in direccion;
	char buffer[BUF_SIZE];
	int pid;


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

/*
       #include <sys/types.h> 
       #include <sys/socket.h>

       int listen(int sockfd, int backlog);
	   		sockfd: socket ID
			backlog: conex concurrentes en cola

*/

	//dejamos escuchando al proceso en el socket ip:puerto
	while(conn_sock=accept(sockid,NULL,NULL)){
		if (conn_sock<0){
			error="accept";
			goto err;
		}

		if (!(pid=fork())){		//proceso hijo
			while(count=recv(conn_sock,buffer,BUF_SIZE,0)){
				if (count < 0){
					error="recv";
					goto err;
				}
	
				*(buffer+count)='\0';
				printf("Recibiendo datos: %s (proceso %d)\n",buffer, getpid());
			}
			close(sockid);
			exit(0);
		}

		printf("Conexion delegada al proceso hijo %d\n", pid);
//		waitpid(pid,NULL,WNOWAIT);
	}



	// ya tenemos un conn_sock de datos asociado con el cliente conectado
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