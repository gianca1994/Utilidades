/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com  / um.edu.ar   *
*******************************************/

#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
#include<string.h>

#define MIN_SIZE 128

int main(int argc, char** argv) {

	int fd,i=0;
	char *buffer;
	struct stat estado;
	fd=open("/tmp/archivo.txt",O_RDWR|O_CREAT,0777);
	printf("%s\n",strerror(errno));



	fstat(fd,&estado);

	if(estado.st_size < MIN_SIZE){
		ftruncate(fd,MIN_SIZE);
	}

	printf("Tamanio: %d\n",estado.st_size);

	if((buffer=mmap(NULL, MIN_SIZE, PROT_WRITE, MAP_SHARED, fd, 0))<0){
		close(fd);
		perror("Error de mapeo");
		printf("%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}


	for(i=0;i<15;i++){
		*(buffer+i)=*argv[1];
		msync(buffer,estado.st_size,MS_ASYNC);
		printf("%s\n",strerror(errno));
		sleep(1);
	}

	munmap(buffer,MIN_SIZE);

	return 0;
}
