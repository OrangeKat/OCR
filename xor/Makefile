# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lm

all: neural_network_xor

SRC = neural_network_xor.c 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

neural_network_xor: neural_network_xor.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
