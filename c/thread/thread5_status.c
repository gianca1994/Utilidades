/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : JuncoTIC - juncotic.com     *
* Contact    : diego@juncotic.com          *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>

// Hilos joinable

void* funcion_int(void*);

int main(int argc, char** argv) {

	char *error;
	char cmd[128];
	int *retorno_int;

	printf("PID: %d\n", getpid());

	pthread_t thread_id;

	printf("Tamanio del id: %ld\n",sizeof(pthread_t));

	if(pthread_create(&thread_id, NULL, funcion_int, NULL)){
		error="pthread_create";
		goto err;
	}

	printf("El id del hilo es: %ld\n",thread_id);

	pthread_join(thread_id,(void**)&retorno_int);
	printf("Valor de retorno del hilo ------------------------------> %d\n",(int)*retorno_int);

	printf("terminando el hilo principal...\n");
	pthread_exit(NULL);

	return 0; //termina el proceso -> tambien terminan todos los hilos

err:
	fprintf(stdout,"%s %d - %s\n",error,errno,strerror(errno));
	exit(1);
}

void* funcion_int(void* dato){
	int *a_retornar;
	a_retornar = (int*)malloc(4);
	*a_retornar = 1234;
	
	sleep(2);
	printf("Ejecutando esta linea desde el hilo (TID: %li)\n",pthread_self());
	printf("terminando el hilo creado...\n");
	pthread_exit(a_retornar);
}
