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

//Hilos detached (mejor rendimiento)

void* funcion(void*);

int main(int argc, char** argv) {

	char *error;
	int info=1234;

	// creamos el id de hilo (pthread_d es un long int)
	pthread_t thread_id;

	printf("Tamanio del id: %ld\n",sizeof(pthread_t));

	//creamos un hilo

/*
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (void *), void *arg);
		*thread: donde almacenar el id del nuevo hilo
		*attr: atributos opcionales para pasarle al hilo (o NULL si no es necesario)
			pthread_attr_init() para cargar los atributos.

		start_routine: funcion ejecutada por el thread
		arg: argumentos pasados a la funcion

*/

	if(pthread_create(&thread_id, NULL, funcion, (void*)&info)){
		error="pthread_create";
		goto err;
	}

	printf("El id del hilo es: %li\n",thread_id);

	//terminamos el hilo principal:
	printf("terminando el hilo principal...\n");
	pthread_exit(NULL);

	return 0; //termina el proceso -> tambien terminan todos los hilos

err:
	fprintf(stdout,"%s %d - %s\n",error,errno,strerror(errno));
	exit(1);
}

void* funcion(void* dato){
	//dato es un argumento pasado al hilo
	//"des-atachamos" el hilo creado del hilo principal
	int dato_en_th;
	pthread_detach(pthread_self());
	sleep(1);
	dato_en_th = *((int*)dato);
	printf("Ejecutando esta linea desde el hilo (TID: %li)\n",pthread_self());
	printf("El dato recibido en el hilo es: %d\n", dato_en_th);
	//terminamos al hilo:
	printf("terminando el hilo creado...\n");
	pthread_exit(NULL);
}