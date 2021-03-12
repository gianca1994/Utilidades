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
#include<sys/wait.h>
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
				printf("%d recibiendo datos: %s",getpid(),buffer);
				printf("%d enviando notificacion\n",getpid());
				count=sprintf(buffer,"Notificacion del proceso %d\n",getpid());
				*(buffer+count)='\0';
				if((send(conn_sock,buffer,count+1,0))<0){
					error="send";
					goto err;
				}

			}
			close(sockid);
			exit(0);
		}

		printf("Conexion delegada al proceso hijo No. %d\n",pid);
		waitpid(pid,NULL,WNOWAIT);
	}



	// ya tenemos un conn_sock de datos asociado con el cliente conectado
	return 0;

err:
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);

}