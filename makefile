# @Author: Alex Jorda
# @Date:   24-10-2017
# @Email:  ajorda@salleurl.edu
# @Project: Práctica LSEat
# @Filename: makefile
# @Last modified by:   Gabriel Cammany Ruiz
# @Last modified time: 30-10-2017

LOGIN1 = ls31343
LOGIN = ls30652
FASE = 2

CC = gcc
COP = -O
CFLAGS = -lpthread -Wall -Wextra

MODULES_PATH = modules/
MANAGERS_PATH = managers/
MODULES_SRC = $(shell find $(MODULES_PATH) -name '*.c')
EXE = picard enterprise data

all: clean $(patsubst %,%.build,$(EXE))

%.build:
	$(CC) $(COP) -o $*/$*.exe $(MODULES_SRC) $(shell find $*/$(MANAGERS_PATH) -name '*.c') $(patsubst %,%.c,$*/$*) $(CFLAGS)

stop:
	ps -u $(LOGIN) | grep $(EXE_CLIENT) | awk '{print $$1}' | xargs kill
	ps -u $(LOGIN) | grep $(EXE_ENTERPRISE) | awk '{print $$1}' | xargs kill
	ps -u $(LOGIN) | grep $(EXE_DATA) | awk '{print $$1}' | xargs kill

clean:
	rm -f $(patsubst %,%.exe,$*)

tar:
	tar -cvf G13_F$(FASE)_$(LOGIN)_$(LOGIN1).tar ./
