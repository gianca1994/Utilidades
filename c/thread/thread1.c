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
	char cmd[128];
	printf("PID: %d\n", getpid());

//	sprintf(cmd, "ps -To pid,tid,tgid,tty,time,comm -p %d", getpid());
//	sprintf(cmd, "ls /proc/%d/task", getpid());
//	sprintf(cmd, "top -Hbn1 -p %d", getpid());
//	sprintf(cmd, "ps -L -p %d", getpid());

	pthread_t thread_id;

	printf("Tamanio del id: %ld\n",sizeof(pthread_t));

	if(pthread_create(&thread_id, NULL, funcion, NULL)){
		error="pthread_create";
		goto err;
	}

	printf("El id del hilo es: %ld\n",thread_id);

	//esperamos a que el hilo "hijo" termine: join (equivalente al wait)
	//system(cmd);
	pthread_join(thread_id,NULL);
		//id del hilo a esperar
		// puntero de retorno, o NULL si el hilo no retorna nada

	//terminamos el hilo principal:
	printf("terminando el hilo principal...\n");
	//exit(1);
	pthread_exit(NULL);

	return 0; //termina el proceso -> tambien terminan todos los hilos

err:
	fprintf(stdout,"%s %d - %s\n",error,errno,strerror(errno));
	exit(1);
}

void* funcion(void* dato){
	sleep(2);
	//dato es un argumento pasado al hilo
	printf("Ejecutando esta linea desde el hilo (TID: %li)\n",pthread_self());
	//terminamos al hilo:
	printf("terminando el hilo creado...\n");
	pthread_exit(NULL);
}

