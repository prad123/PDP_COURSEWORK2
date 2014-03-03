MF=	Makefile

# For ness
CC=	mpic++

# For HECToR
# CC=	cc

CFLAGS=	-g -O3 

LFLAGS=	-lm

EXE=	sim

INC= \
#	No user-defined include files at present - list here if required.

SRC= \
	main.cpp actor.cpp frog.cpp cell.cpp timer.cpp environment.cpp disease_sim_factory.cpp pool.c frog-functions.c ran2.c

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(OBJ):	$(INC)

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF)

clean:
	rm -f $(OBJ) $(EXE) core
