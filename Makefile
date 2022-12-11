# Makefile

CC = gcc
CPPFLAGS = -Wno-cpp
CFLAGS = -Wall -Wextra -lm -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = 
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all:  grayscale detectAllLines solver rotation autorotation preprocess

SRC =  grayscale.c detectAllLines.c rotation.c solver.c autorotation.c preprocess.c 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

autorotation: autorotation.o
rotation: rotation.o
grayscale: grayscale.o
preprocess: preprocess.o
detectAllLines: detectAllLines.o
solver: solver.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} number*.png
	${RM} grid.png
	${RM} grayscale.png
	${RM} rot_done.png
	${RM} grid_00.result
	${RM} preprocess.png
	
# END
