# @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
# @Date:   24-10-2017
# @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
# @Project: Práctica LSEat
# @Filename: makefile
# @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
# @Last modified time: 26-10-2017



EXE_CLIENT = Picard
LOGIN1 = ls31343
LOGIN2 = ls30652
CC = gcc
CFLAGS = -lpthread -Wall -Wextra
COP = -o
MAIN_CLIENT = main.c
EXTRA_LIBS = fitxers.o shell.o utils.o

all: clean fitxers utils shell $(EXE_CLIENT) 

fitxers: fitxers.c types.h utils.h
	$(CC)  fitxers.c -c $(CFLAGS)

utils: utils.c types.h
	$(CC)  utils.c -c $(CFLAGS)

shell: shell.c types.h utils.o
	$(CC)  shell.c -c $(CFLAGS)

$(EXE_CLIENT): $(EXTRA_LIBS)
	$(CC) $(COP) $(EXE_CLIENT) $(MAIN_CLIENT) $(EXTRA_LIBS) $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill

clean:
	rm -f $(EXE_CLIENT) $(EXTRA_LIBS) $(EXTRA_LIBS)

cextra:
	rm -f $(EXTRA_LIBS)
