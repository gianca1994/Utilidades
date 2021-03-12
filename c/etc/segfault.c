/* Este codigo seguramente produzca un segmentation fault :) */
/* Compilar en línea de comandos con:
 * gcc segfault.c -o segfault
 *
 * Ejecutar con:
 * ./segfault
 */

int main(int argc, const char *argv[]){
	
	int *ptr;
	*(ptr + 1000000) = 10;

	return 0;
}