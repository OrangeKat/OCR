# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags cairo`
LDLIBS = `pkg-config --libs cairo`

all:  tticonvert

SRC =  tticonvert.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

tticonvert: tticonvert.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} img.png
# END
