
OPT = -g -Wall -O2
CC = gcc

all: labexplr

labexplr: labexplr.c labexplr.h makefile
	${CC} ${OPT} labexplr.c -o labexplr




