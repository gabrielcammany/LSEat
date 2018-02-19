LOGIN1 = ls31343
LOGIN = ls30652
FASE = 3

CC = gcc
COP =
CFLAGS = -lpthread -Wall -Wextra -g

LIBS_PATH = lib/src/
LIBS_SRC = $(shell find $(LIBS_PATH) -name '*.c')
EXE = enterprise data picard

all: clean $(patsubst %,%.build,$(EXE))

%.build:
	$(CC) $(COP) -o bin/$*.exe $(LIBS_SRC) $(shell find $*/src/ -name '*.c') $(patsubst %,%.c,$*/$*) $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill
	ps -u $(LOGIN) | grep $(EXE_ENTERPRISE) | awk '{print $$1}' | xargs kill
	ps -u $(LOGIN) | grep $(EXE_DATA) | awk '{print $$1}' | xargs kill

clean:
	mkdir -p bin
	rm -f $(patsubst %,%.exe,$*)

tar:
	tar -cvf G13_$(LOGIN)_$(LOGIN1).tar ./
