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
#include<netinet/in.h>
#include<sys/socket.h>

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
	dir.sin_addr.s_addr=inet_addr(*(argv+1));


	while(count=read(STDIN_FILENO,buffer,BUF_SIZE)){
		*(buffer+count)='\0'; // importante para la comparacion
		if((sendto(sockid, buffer, count+1,0, (struct sockaddr *)&dir, sizeof(dir)))<0){
			error="sendto";
			goto err;
		}
		printf("Enviado...(%s - %d)\n",buffer,count);
		if(!strcmp(buffer,"exit\n")){
			printf("Saliendo del cliente...\n");
			break;
		}
	}

	close(sockid);

	return 0;
err:
	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
}
