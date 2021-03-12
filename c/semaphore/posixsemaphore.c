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


#define SEM_NAME "/sema"

int main(int argc, char **argv){
	char comando[10];
	int i;
	int valor;
	sem_t *mutex;




	sem_unlink(SEM_NAME);
	mutex = sem_open(SEM_NAME, O_CREAT, 0666, 1);

	sem_getvalue(mutex, &valor); printf("Valor actual: %d\n", valor);


	if(fork()==0){
		sleep(1);
		for(i=0;i<3;i++){
			sem_wait(mutex);
			sem_getvalue(mutex, &valor); printf("Valor actual: %d (hijo adentro)\n", valor);

			printf("Hijo usando recurso...\n");
			sleep(1);
			printf("Hijo liberando recurso \n\n");
			
			sem_post(mutex);
			sem_getvalue(mutex, &valor); printf("Valor actual: %d (hijo saliendo)\n", valor);
			usleep(100-i*10);
		}
		return (0);
	}
	for(i=0;i<3;i++){
		sem_wait(mutex);
		sem_getvalue(mutex, &valor); printf("Valor actual: %d (padre adentro)\n", valor);
	
		printf("Padre usando recurso...\n");
		sleep(2);
		printf("Padre liberando recurso \n\n");
		
		sem_post(mutex);
		sem_getvalue(mutex, &valor); printf("Valor actual: %d (padre saliendo)\n", valor);
		usleep(100+i*10);
	}
	wait(NULL);
	sem_close(mutex);
	sem_unlink(SEM_NAME);

	return (0);
}

