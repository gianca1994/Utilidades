/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Contact    : juncotic.com - um.edu.ar    *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 512

int main(int argc, char** argv) {

	char *error;
	struct sockaddr_in dir;
	int sockid,count;
	char buffer[BUF_SIZE];
	
	//creamos el socket:
	if((sockid=socket(PF_INET,SOCK_DGRAM,0))<0){
		error="socket";
		goto err;
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
*/

	dir.sin_family=AF_INET;
	dir.sin_port = htons(1234);
	dir.sin_addr.s_addr=htonl(INADDR_ANY); // 0.0.0.0
		//estaria levantando el server en udp:0.0.0.0:1234

	/*
		htons - host to network (short 16b)	
		htonl - host to network (long 32b)
		ntohs ntohl - network to host short/long
		ENDIAN:
			big-endian (mas significativos en direcciones bajas)		host
			litle-endian (menos significativos en direcciones bajas)		network
	*/

	//asociamos la direccion con el socket
	if((bind(sockid, (struct sockaddr *)&dir,sizeof(dir)))<0){
		error="bind";
		goto err;
	}
	
	while(count=recv(sockid,buffer,BUF_SIZE,0)){
		if(count<0){
			error="recv";
			goto err;
		}

		if(!strcmp(buffer,"exit\n")){
			printf("Terminando servidor...\n");
			break;
		}

		printf("Recibiendo datos: %s",buffer);
	}

	close(sockid);

	return 0;
err:
	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
}

