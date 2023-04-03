#GLOBALES
CC = gcc #defino variable, luego la llamo con $
OUTFILE = Swap.out

SRC = $(shell find ./src -type f -iname "*.c" -exec basename {} \;)
OBJ = $(SRC:%.c=./obj/%.o)
#ejemplo de lo que hace OBJ
#sumas.c restas.c
#./obj/sumas.c ./obj/restas.c

SHARED = $(shell find ../Shared/obj/*.o)
LINK = -lcommons -lpthread -lreadline -lm -lbiblio
CFLAGS = -c

#build contiene main.c + los objetos del propio modulo + los de la SHARED
build: makefolder makeshared $(OBJ) #dependencias
	$(CC) -o $(OUTFILE) swap.c $(OBJ) $(SHARED) $(LINK)

#VALGRIND 
valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind.log ./$(OUTFILE)

debug: CFLAGS := $(CFLAGS) -g
debug: makeshared_debug build

#   $@         $<
./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) $< -o $@ $(LINK)


#.PHONY = makefolder   esto seria para que ejecute siempre esta regla
#makefolder -> crea la carpeta donde guardamos todos nuestros .o
makefolder:
	if [ ! -d ./obj ]; then mkdir obj; fi 
#crea el directorio obj si no existe

#compila la carpeta compartida de los modulos
makeshared:
	make -C ../Shared

makeshared_debug:
	make debug -C ../Shared

#CLEAN
clean:
	rm -rf obj
	rm -rf *.out
	rm -rf *.log
	make clean -C ../Shared