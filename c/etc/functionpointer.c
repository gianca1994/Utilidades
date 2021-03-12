/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Purpose    : juncotic.com                *
* Contact    : @d1cor / @juncotic          *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<errno.h>

int suma(int n1,int n2);
int resta(int n1,int n2);
int mult(int n1,int n2);

int main(int argc, char** argv) {

//calculadora que suma, multiplica y resta

	int num1,num2,result;
	char op;

	//definimos un puntero para funciones con formato
	//	int nombre_func(int,int)
	int (*operacion)(int,int);


	printf("ingrese una operacion: (ej, 2+3)  + - *\n");
	scanf("%d%c%d",&num1,&op,&num2);

	switch(op){
	case '+':
		operacion=suma;	//a op le cargamos la dir de mem de la funcion suma
		break;
	case '-':
		operacion=resta;
		break;
	case '*':
		operacion=mult;
		break;
	default:
		printf("Error de carga de datos\n");
		exit(2);
	}

	result=(*operacion)(num1,num2);

	printf("El resultado es: %d\n",result);

	return 0;
}

int suma(int n1,int n2){
	return n1+n2;
}
int resta(int n1,int n2){
	return n1-n2;
}
int mult(int n1,int n2){
	return n1*n2;
}