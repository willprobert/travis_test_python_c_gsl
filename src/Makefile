C = gcc

OBJS = main.o
GSLFLAGS= -lgsl -lgslcblas -lm -O3
LFLAGS = ${GSLFLAGS}

# Name of executable
_EXE = watts_strogatz.exe
EXE = $(_EXE)

# Libraries
INC = /usr/local/include
LIB = /usr/local/lib

# Compilation options and libraries to be used
CFLAGS = -g -Wall -fmessage-length=0 -I$(INC) -O0

# To compile
all: ${OBJS}
	${C} -L${LIB} -o ${EXE} ${OBJS} ${LFLAGS}

clean:
	rm -f $(OBJS) $(EXE)

.c.o:
	${C} ${CFLAGS} -c $< -o $@	
