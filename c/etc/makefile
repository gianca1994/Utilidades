APP=calc
CC=gcc
DEPS=../include/calc.h
CFLAGS=-I ../include/
ODIR=../obj

OBJ=../obj/calc.o ../obj/suma.o ../obj/resta.o ../obj/mult.o ../obj/division.o ../obj/datos.o


calc:	$(OBJ)
	$(CC) -o $(APP) $(OBJ)


$(ODIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# % se sustituye con el nombre del archivo
# $@ lo que esta a la izquierda del ':'
# $< el primer argumento de la derecha del ':'
# $^ toda la lista de la derecha de ':'

clean:
	rm -f calc $(ODIR)/*.o