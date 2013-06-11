#=================================================
#|||||||||||||||| MAC 323 - EP3 ||||||||||||||||||
#||||| Nome: Carlos Eduardo Leao Elmadjian |||||||
#||||| NUSP: 5685741 |||||||||||||||||||||||||||||
#||||| Arquivo: makefile |||||||||||||||||||||||||
#=================================================
CFLAGS=-Wall -ansi -pedantic
CC=gcc
#=================================================
#
ep3: main.o LLRBT.o
	$(CC) $(CFLAGS) LLRBT.o main.o -o ep3 -lm && rm *.o

main.o: main.c LLRBT.h
	$(CC) $(CFLAGS) -c main.c

LLRBT.o: LLRBT.c LLRBT.h
	$(CC) $(CFLAGS) -c LLRBT.c

