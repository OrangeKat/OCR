#Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -g `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = 
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: split_grid

SRC = split_grid.c split_grid.h
OBJ = ${SRC: .c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
