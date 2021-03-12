/*******************************************
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com  / um.edu.ar   *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include <semaphore.h>
#include <wait.h>
#include <pthread.h>


#define SEM_NAME "/sema"

void * funcion(void *p);

int main(int argc, char ** argv){
	int i;
	int valor;
	sem_t mutex;
	pthread_t tid;	


	sem_init(&mutex,0,1);


	sem_getvalue(&mutex, &valor); printf("Valor actual: %d\n", valor);
	if(pthread_create(&tid, NULL, funcion, (void*)&mutex)){
		printf("\n ERROR creating thread 1");
		exit(1);
	}

	

	//padre
	for(i=0;i<3;i++){
		sem_wait(&mutex);
		sem_getvalue(&mutex, &valor); printf("Valor actual: %d (padre adentro)\n", valor);
	
		printf("Padre usando recurso...\n");
		sleep(2);
		printf("Padre liberando recurso \n\n");
		
		sem_post(&mutex);
		sem_getvalue(&mutex, &valor); printf("Valor actual: %d (padre saliendo)\n", valor);
		usleep(100+i*10);
	}
	
	if(pthread_join(tid, NULL)){
		printf("\n ERROR joining thread");
		exit(1);
	}
	
	// cierra el semaforo
	sem_close(&mutex);

	return (0);
}


void * funcion(void *p){
	int i, valor;
	sem_t *mutex = (sem_t*)p;
	
	for(i=0;i<3;i++){
		sem_wait(mutex); //baja
		sem_getvalue(mutex, &valor); printf("Valor actual: %d (hijo adentro)\n", valor);

		printf("Hijo usando recurso...\n");
		sleep(1);
		printf("Hijo liberando recurso \n\n");
	
		sem_post(mutex); //sube
		sem_getvalue(mutex, &valor); printf("Valor actual: %d (hijo saliendo)\n", valor);
		usleep(100-i*10);
	}
}