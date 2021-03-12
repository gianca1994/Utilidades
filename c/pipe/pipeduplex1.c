/*
 * Author: - @d1cor - juncotic.com
 * Date: 07/jun/2017
 */

#include<stdio.h>
#include<unistd.h>
#include<wait.h>

#define BUF_SIZE 1024

int main(int argc, char** argv){

	int tubo[2];
	char buf[BUF_SIZE];
	pipe(tubo);

	if(!fork()){
		int count = read(tubo[0],buf,BUF_SIZE);
		*(buf+count)='\0';
		printf("Hijo R:%s\n",buf);

		write(tubo[1],"hijo listo",10);

		close(tubo[0]);
		close(tubo[1]);
		_exit(0);

	}

	write(tubo[1],"padre enviando",14);
	sleep(1);
	int count = read(tubo[0],buf,BUF_SIZE);
	*(buf+count)='\0';
	printf("Padre R:%s\n",buf);
	close(tubo[0]);
	close(tubo[1]);

	return 0;
}