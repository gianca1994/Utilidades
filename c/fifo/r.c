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

	if((tuberia=open("/tmp/mififo",O_RDONLY))<0){
		error="open";
		goto err;
	}

	printf("LECTOR\tLeyendo...\n");

	while(count=read(tuberia,cadena,BUF_SIZE)){
		*(cadena+count)='\0';
		printf("LECTOR\t%s\n",cadena);
	}

	close(tuberia);


	unlink(*(argv+1));

	return 0;

err:
	fprintf(stderr,"%s: %d-%s\n",error,errno,strerror(errno));
	exit(1);
}
