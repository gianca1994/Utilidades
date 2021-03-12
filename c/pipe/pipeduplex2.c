/*
 * Author: - @d1cor - juncotic.com
 * Date: 07/jun/2017
 */

#include<stdio.h>
#include<unistd.h>
#include<wait.h>

#define BUF_SIZE 1024

int main(int argc, char** argv){

	int p_h[2];
	int h_p[2];
	char buf[BUF_SIZE];
	pipe(p_h);
	pipe(h_p);

	if(!fork()){
		close(p_h[1]);
		close(h_p[0]);
		int count = read(p_h[0],buf,BUF_SIZE);
		*(buf+count)='\0';
		printf("Hijo R:%s\n",buf);

		write(h_p[1],"hijo listo",10);

		close(p_h[0]);
		close(h_p[1]);
		_exit(0);

	}
	close(p_h[0]);
	close(h_p[1]);

	write(p_h[1],"padre enviando",14);
	int count = read(h_p[0],buf,BUF_SIZE);
	*(buf+count)='\0';
	printf("Padre R:%s\n",buf);
	close(h_p[0]);
	close(p_h[1]);

	return 0;
}