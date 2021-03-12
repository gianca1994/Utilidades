/**
 * @File: calculadora.c
 * @Created: 07 nov 2016
 * @Author: @d1cor - Diego Cordoba / juncotic.com
 * @Descr: Ejemplo de calculadora monolitica
 * @Modified: 28 mar 2018
 */


#include<stdlib.h>
#include<stdio.h>

float suma(float,float);
float resta(float,float);
float mult(float,float);
float division(float,float);
char datos(float*,float*);


int main(int argc, char** argv) {

	float num1,num2, resultado;
	char op;

	op=datos(&num1,&num2);

	switch(op){
	case '+':
		resultado=suma(num1,num2);
		break;
	case '-':
		resultado=resta(num1,num2);
		break;
	case '*':
		resultado=mult(num1,num2);
		break;
	case '/':
		resultado=division(num1,num2);
		break;
	default:
		printf("Opcion incorrecta\n");
		goto no_result;
	}

	printf("Resultado: %.2f\n",resultado);

no_result:

	return 0;
}


float suma(float n1,float n2){
	return n1+n2;
}


float resta(float n1,float n2){
	return n1-n2;
}


float mult(float n1,float n2){
	return n1*n2;
}


float division(float n1,float n2){
	return n1/n2;
}


char datos(float *num1, float *num2){
	printf("Introduzca valor 1: ");
	scanf("%f",num1);

	printf("Introduzca valor 2: ");
	scanf("%f",num2);

	printf("Introduzca a operacion: (+ - * /) ");
	getchar();
	return getchar();
}