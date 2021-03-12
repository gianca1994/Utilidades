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

void* funcion(void*);

int main(int argc, char** argv) {

	char *error;

	int status = 0;

	// creamos el id de hilo (pthread_d es un long int)
	pthread_t thread_id;

	printf("Tamanio del id: %ld\n",sizeof(pthread_t));

	//creamos un hilo

	if(pthread_create(&thread_id, NULL, funcion, NULL)){
		error="pthread_create";
		goto err;
	}

	printf("El id del hilo es: %ld\n",thread_id);

	pthread_join(thread_id,(void*)&status);
	printf("Estado de terminacion: %d\n", status);

	//terminamos el hilo principal:
	printf("terminando el hilo principal...\n");
	pthread_exit(NULL);

	return 0; //termina el proceso -> tambien terminan todos los hilos

err:
	fprintf(stdout,"%s %d - %d\n",error,errno,strerror(errno));
	exit(1);
}

void* funcion(void* dato){
	sleep(2);
	printf("Ejecutando esta linea desde el hilo (TID: %li)\n",pthread_self());
	printf("terminando el hilo creado...\n");
	return (void**)1122;
}

