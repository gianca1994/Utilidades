/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com / um.edu.ar    *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

#define TIMER 10
#define LOOPS 5

void cambiar_pid(){
	pid_t pid;
	pid = fork();
	if(pid){ // el padre
		exit(0);
	}
}


int main(int argc, char** argv) {

	char buffer[1024];
	char nombre[1024];
	char *error;
	char *nombre_tmp;
	FILE *fd;
	int i;
	int nombre_tmp_len;
	pid_t pid;

	memset(buffer, '\0', sizeof(buffer));
	memset(nombre, '\0', sizeof(nombre));

	//seteamos un identificador para el nombre (para poder verlo en la lista):
	*(nombre)='_';
	*(nombre+1)='_';
	*(nombre+2)='_';

	//obtenemos la lista de procesos del sistema para leer los nombres:
	system("/bin/sh -c 'ps fax > /tmp/procesos.txt'");

	//abrimos el archivo de procesos
	if((fd=fopen("/tmp/procesos.txt","r"))==NULL){
		error="fopen";
		goto err;
	}

	fgets(buffer, sizeof(buffer), fd);
	while(fgets(buffer, sizeof(buffer), fd)){
		printf("Buffer: %s\n",buffer);
		nombre_tmp=strchr(buffer,'[');

		if(nombre_tmp == NULL){
			strcpy(nombre_tmp, "/bin/bash");
		}

		nombre_tmp_len = strlen(buffer);

		strcat((nombre+3),nombre_tmp);

		strncpy(*argv, nombre,strlen(nombre));

		cambiar_pid();

		// "tareas maliciosas"
		sleep(TIMER);
		memset((nombre+3),'\0',sizeof(nombre-3));
	}
	fclose(fd);

	return 0;

err:
	fprintf(stderr,"%d (%s) - %s\n",errno, error, strerror(errno));
	exit(2);
}


