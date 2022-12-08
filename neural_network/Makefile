# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = -lm  `pkg-config --libs sdl2 SDL2_image`

all: neural_network

SRC = neural_network.c binarization.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}



.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
