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
MAIN_CLIENT = client.c
EXTRA_LIBS = fitxers.o shell.o utils.o
FOLDER = modules/

all: clean fitxers utils shell $(EXE_CLIENT) 

fitxers: $(FOLDER)fitxers.c $(FOLDER)types.h $(FOLDER)utils.h
	$(CC)  $(FOLDER)fitxers.c -c $(CFLAGS)

utils: $(FOLDER)utils.c $(FOLDER)types.h
	$(CC)  $(FOLDER)utils.c -c $(CFLAGS)

shell: $(FOLDER)shell.c $(FOLDER)types.h $(FOLDER)utils.o
	$(CC)  $(FOLDER)shell.c -c $(CFLAGS)

$(EXE_CLIENT): $(EXTRA_LIBS)
	$(CC) $(COP) $(EXE_CLIENT) $(MAIN_CLIENT) $(EXTRA_LIBS) $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill

clean:
	rm -f $(EXE_CLIENT) $(EXTRA_LIBS) $(EXTRA_LIBS)

cextra:
	rm -f $(EXTRA_LIBS)
