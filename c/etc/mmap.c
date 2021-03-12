/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : UM - juncotic.com           *
* Title      : Ejemplo modificacion archivo*
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

int main(int argc, char** argv) {

	int fd1=0;
	char *buff;
	int i;
	char dst[100];
	char *ini;

	// abrimos el file en modo lectura-escritura
	// este archvio originalmente podria contener, por ejemplo, esta cadena:
	// 123456789-_A_-987654321

	fd1=open("/tmp/archivo.txt",O_RDWR,(mode_t)0666);
	if(!fd1) goto err;

	// mapeamos la memoria en el puntero buff
	if((buff=mmap(NULL,50,PROT_WRITE,MAP_SHARED,fd1,0))==MAP_FAILED){
		close(fd1);
		perror("Error de mapeo");
		exit(EXIT_FAILURE);
	}

	// imprimimos por pantalla el contenido del archivo utilizando buff
	printf("El contenido del archivo es:%s, len: %d\n",buff, strlen(buff));

	// obtenemos la longitud de la memoria (y por ende, del archivo :-) )
	int len = strlen(buff);

	// ciclamos mientras no alcancemos la longitud máxima
	// vamos corriendo el puntero hasta encontrar la letra A
	for(i=0; i<len; i++){
		if (*(buff+i) == 'A'){
			printf("Encontrado en %d\n",i);
			ini = buff+i;
			break;
		}
		printf("Buscando: %c (lenght de lo que queda: %d\n",*(buff+i), strlen((buff+i)));
		sleep(1);
	}
	// cuando se encontró la letra A, ini va a contener el puntero al inicio del bloque de datos que inicia con "A"
	// e "i" va a tener la cantidad de bytes que hay hasta el caracter "A"

	// movemos a dst el bloque de datos anterior a "A"
	memmove(dst, buff, i);

	// concatenamos a ese bloque, el bloque que sigue a "A"
	memcpy(dst+i, buff+i+1, len-i);

	// uso "buff+i+1" para "saltar" el caracter "A", pero puede reemplazarse el 1
	// con los N chars que correspondan a la longitud de los datos a eliminar

	printf("DST: %s\n",dst);
	// la cadena resultante deberia haber eliminado la A
	// 123456789-__-987654321

	// limpiamos la memoria original del puntero / archivo
	memset(buff,0,len);

	// movemos la memoria modificada a la ubilicacion de buff, que es la dir de memoria mapeada
	memmove(buff,dst,len-1);
	// lo mismo, 1 se reemplaza con N bytes si se eliminan N en vez de 1.

	munmap(buff,len);
	close(fd1);
	return 0;

err:
	fprintf(stderr,"(%d) %s\n",errno,strerror(errno));
	exit(errno);
}