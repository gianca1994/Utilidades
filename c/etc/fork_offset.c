#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define ARCH "salida.txt"
int main(int argc, char** argv){
	int status;
	int fd=open(ARCH, O_RDWR|O_CREAT|O_TRUNC, 0666);
	if (fd<0) {
		perror(ARCH);
		return 1;
	}

	write(fd, "Comienzo ...", 12);

	switch(fork()) {
		case -1: perror("fork()");return 255;
		case 0:
			/* hijo */
			if (lseek(fd, 1024, SEEK_SET)<0) {
				perror("lseek");
				return 1;
			}
			write(fd, "HIJO: ->", 8);
			printf("Offset del hijo: %d\n", lseek(fd, 0, SEEK_CUR));
			return 0;
	}
	wait(&status);
	write(fd, "Donde-stoy?", 11);
	printf("Offset del padre: %d\n", lseek(fd, 0, SEEK_CUR));
	return 0;
}

