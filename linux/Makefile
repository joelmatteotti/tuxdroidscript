EXEC=TDSInterpreter
SRCDIR=../src/
OBJDIR=../obj/
INCDIR=../include/


CC=gcc

CLFAGS=-I${INCDIR} -ansi -pedantic --std=c99 -Wall -pipe -O2 -fno-stack-protector -D_FORTIFY_SOURCE=0

.PHONY: clean mrproper

all: ${EXEC}

${EXEC}: TuxStrings.o TDSInterpreter.o


TuxStrings.o: ${SRCDIR}TuxStrings.c
	mkdir -p ${OBJDIR}
	${CC} ${CFLAGS} -o ${OBJDIR}TuxStrings.o -c ${SRCDIR}TuxStrings.c


TDSInterpreter.o: ${SRCDIR}TDSInterpreter.c
	mkdir -p ${OBJDIR}
	${CC} ${CFLAGS} -o ${OBJDIR}TDSInterpreter.o -c ${SRCDIR}TDSInterpreter.c


clean:
	rm -f ${OBJDIR}*

mrproper: clean
	rm -Rf ${OBJDIR}
	rm -f ${EXEC}
