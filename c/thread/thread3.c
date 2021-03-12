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

#define NUM_TH 10

void* funcion(void*);

int main(int argc, char** argv) {

	char *error;
	int info[NUM_TH],i;

	// creamos el id de hilo (pthread_d es un long int)
	pthread_t thread_id[NUM_TH];

	printf("Tamanio del id: %ld\n",sizeof(pthread_t));

	//creamos NUM_TH hilos, y le pasamos a cada uno, un arg

	printf("creando el hilo principal...\n");

	printf("el hilo principal crea al resto\n");
	for(i=0;i<NUM_TH;i++){
		printf("El hilo principal crea al hilo %d\n",i);
		*(info+i) = i;
		if(pthread_create((thread_id+i), NULL, funcion, (void*)(info+i))){
			error="pthread_create";
			goto err;
		}
		//sleep(1);
	}

/*	for(i=0;i<NUM_TH;i++){
		pthread_join(*(thread_id+i),NULL);
		printf("El hilo %d termino\n",i);
	}
*/
	//terminamos el hilo principal:
	printf("terminando el hilo principal...\n");
	pthread_exit(NULL);

	return 0; //termina el proceso -> tambien terminan todos los hilos

err:
	fprintf(stdout,"%s %d - %s\n",error,errno,strerror(errno));
	exit(1);
}

void* funcion(void* dato){
	int dato_en_th;
	pthread_detach(pthread_self());
	sleep(1);
	dato_en_th = *((int*)dato);
	printf("Ejecutando el hilo %d\n",dato_en_th);
	printf("terminando el hilo creado %d...\n",dato_en_th);
	pthread_exit(NULL);
}