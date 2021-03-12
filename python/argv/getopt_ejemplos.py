#!/usr/bin/python3.4

import getopt
import sys

#getopt reconoce la entrada de linea de comandos y la parsea por nosotros

#sintaxis: getopt.getopt(argumentos, "opc_un_char" [, "opc_palabra"])
# getopt.getopt(sys.argv[1:], "abc", "hola")

#opciones validas: -a, -b -c

#(opt,arg) = getopt.getopt(sys.argv[1:], 'ab:c', ["hola", "mundo="])
# -a -b -c


#(opt,arg) = getopt.getopt(sys.argv[1:], 'ab:c')
# -a -b algo -c
# ./getopt.py -a -b 123 -c


#(opt,arg) = getopt.getopt(sys.argv[1:], 'a:b:c')
# -a algo -b algo -c


#(opt,arg) = getopt.getopt(sys.argv[1:], 'ab:c', ["opcion1", "opcion2"])
# -a -b algo -c --opcion1 --opcion2


(opt,arg) = getopt.getopt(sys.argv[1:], 'ab:c', ["agregar", "opcion1", "opcion2="])
# -a -b algo -c --opcion1 --opcion2="holamundo"

print("opciones: ", opt)
print("argumentos: ", arg)

#parseamos los argumentos usando un for:

for (op,ar) in opt:
	if(op in ['-a', 'agregar']):
            print("Opcion -a o --agregar seteada")
	elif(op == '-b'):
		print("Opcion -b seteada, argumento: ", ar)
	elif(op == '-c'):
		print("Opcion -c seteada")
	elif(op == "opcion1"):
		print("Opcion --opcion1 seteada")
	elif(op == "opcion2"):
		print("Opcion --opcion2 seteada, argumento: ", ar)
	else:
		print("Opcion invalida")

