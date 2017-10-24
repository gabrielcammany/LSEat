EXE_CLIENT = VaultTec-Client
EXE_SRV = VaultTec-Server
LOGIN = gabriel
CC = gcc
CFLAGS = -lpthread -Wall -Wextra
COP = -o
MAIN_CLIENT = client.c
MAIN_SRV = servidor.c
EXTRA_LIBS = basicUtils.o socketUtils.o

all: clean basicUtils socketUtils $(EXE_CLIENT) $(EXE_SRV)

$(EXE_CLIENT):
	$(CC) $(COP) $(EXE_CLIENT) $(MAIN_CLIENT) $(EXTRA_LIBS) $(CFLAGS)

$(EXE_SRV):
	$(CC) $(COP) $(EXE_SRV) $(MAIN_SRV) $(EXTRA_LIBS) $(CFLAGS)

basicUtils: basicUtils.c basicUtils.h
	$(CC) -c basicUtils.c $(CFLAGS)

socketUtils: socketUtils.c socketUtils.h
	$(CC) -c socketUtils.c $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill
	ps -u $(LOGIN) | grep $(EXE_SRV) | awk '{print $$1}' | xargs kill

clean:
	rm -f $(EXE_CLIENT) $(EXE_SRV) $(EXTRA_LIBS)

cutils:
	rm -f $(EXTRA_LIBS)
