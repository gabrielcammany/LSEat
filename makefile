# @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
# @Date:   24-10-2017
# @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
# @Project: Práctica LSEat
# @Filename: makefile
# @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
# @Last modified time: 26-10-2017



EXE_CLIENT = Picard
#EXE_SRV =
LOGIN1 = ls31343
LOGIN2 = ls30652
CC = gcc
CFLAGS = -lpthread -Wall -Wextra
COP = -o
MAIN_CLIENT = main.c
#MAIN_SRV = servidor.c
EXTRA_LIBS = main.o fitxers.o shell.o utils.o

all: clean $(EXE_CLIENT) $(EXTRA_LIBS)# $(EXE_SRV)

#$(EXE_SRV):
#	$(CC) $(COP) $(EXE_SRV) $(MAIN_SRV) $(EXTRA_LIBS) $(CFLAGS)

main.o: main.c types.h fitxers.h shell.h
	$(CC)  main.c -c $(CFLAGS)

fitxers.o: fitxers.c types.h utils.h
	$(CC)  fitxers.c -c $(CFLAGS)

utils.o: utils.c types.h
	$(CC)  utils.c -c $(CFLAGS)

shell.o: shell.c types.h utils.o
	$(CC)  shell.c -c $(CFLAGS)

$(EXE_CLIENT): $(EXTRA_LIBS)
	$(CC) $(COP) $(EXE_CLIENT) $(EXTRA_LIBS) $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill
	#ps -u $(LOGIN) | grep $(EXE_SRV) | awk '{print $$1}' | xargs kill

clean:
	rm -f $(EXE_CLIENT) $(EXTRA_LIBS)# $(EXE_SRV) $(EXTRA_LIBS)

cutils:
	rm -f $(EXTRA_LIBS)
