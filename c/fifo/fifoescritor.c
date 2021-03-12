/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com @juncotic      *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

#define BUF_SIZE 256

int main(int argc, char** argv) {

	char *error;
	char cadena[BUF_SIZE];
	int count,i;
	int tuberia;	//file descriptor del fifo


	if((tuberia=open("/tmp/archivo_fifo",O_WRONLY))<0){
		error="open";
		goto err;
	}


	printf("ESCRITOR\tEscribiendo...%d\n",tuberia);

	count=sprintf(cadena,"ESCRITOR\thola proceso\n");
	write(tuberia,cadena,count);
	sleep(1);

	count=sprintf(cadena,"ESCRITOR\tsigo escribiendo...\n");
	write(tuberia,cadena,count);

	for(i=0;i<10;i++){
		printf("escribiendo %d\n",i);
		count=sprintf(cadena,"ESCRITOR\tescribiendo mensaje %d\n",i);
		write(tuberia,cadena,count);
		sleep(1);
	}

	close(tuberia);

	return 0;

err:
	fprintf(stderr,"%s: %d-%s\n",error,errno,strerror(errno));
	exit(1);
}


// https://tmate.io/t/ro-1c5ttPEMJsa0FZt3ej1iSWXWx