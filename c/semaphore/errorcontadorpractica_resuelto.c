/*******************************************
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com  / um.edu.ar   *
*******************************************/

/*
 * Corregir este codigo para que devuelva el resutltado correcto
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NITER 1000000
#define NTHREADS 5

struct params{
	int cnt;
	sem_t mutex;
};

typedef struct params params_t;

void* Count(void*);

int main(int argc, char ** argv){
	params_t p;
	pthread_t tid[NTHREADS];

	int i=0;

	p.cnt = 0;
	sem_init(&(p.mutex),0,1);

	for(i=0; i<NTHREADS; i++){
		if(pthread_create((tid+i), NULL, Count, (void*)&p)){
			printf("\n ERROR creating thread");
			exit(1);
		}
	}

	for(i=0; i<NTHREADS; i++){
		if(pthread_join(*(tid+i), NULL)){
			printf("\n ERROR joining thread");
			exit(1);
		}
	}

    if (p.cnt < NTHREADS * NITER)
        printf("\n NOOOOOO! cnt es [%d], deberia ser %d\n", p.cnt, NTHREADS*NITER);
    else
        printf("\n BIEN! cnt es [%d]\n", p.cnt);

    pthread_exit(NULL);
}

void* Count(void *p){
    int i, tmp;
	params_t *pp = (params_t*) p;
	
    for(i = 0; i < NITER; i++){
	    sem_wait(&(pp->mutex));
			tmp = pp->cnt;
	    tmp = tmp+1;
	    pp->cnt = tmp;
	    sem_post(&(pp->mutex));
    }
}
