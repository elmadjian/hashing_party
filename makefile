#=================================================
#|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||
#||||| Nome: Carlos Eduardo Leao Elmadjian |||||||
#||||| NUSP: 5685741 |||||||||||||||||||||||||||||
#||||| Arquivo: makefile |||||||||||||||||||||||||
#=================================================
CFLAGS=-Wall -ansi -pedantic -g
CC=gcc
#=================================================
#
# NAO ESQUECER DE MODULARIZAR TUDO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ep4encenc: main.o t1enc.o
	$(CC) $(CFLAGS) t1enc.o main.o -o ep4encenc -lm && rm *.o

#ep4enclp: main.o t1enc.o t2lp.o
#	$(CC) $(CFLAGS) t1enc.o t2lp.o main.o -o ep4enclp -lm && rm *.o

#ep4lpenc: main.o t1lp.o t2enc.o
#	$(CC) $(CFLAGS) t1lp.o t2enc.o main.o -o ep4lpenc -lm && rm *.o
#
#ep4lplp: main.o t1lp.o t2lp.o
#	$(CC) $(CFLAGS) t1lp.o t2lp.o main.o -o ep4lplp -lm && rm *.o

main.o: main.c interface.h
	$(CC) $(CFLAGS) -c main.c

t1enc.o: t1enc.c interface.h
	$(CC) $(CFLAGS) -c t1enc.c

#t2enc.o: t2enc.c interface.h
#	$(CC) $(CFLAGS) -c t2enc.c
#
#t1lp.o: t1lp.c interface.h
#	$(CC) $(CFLAGS) -c t1lp.c
#
#t2lp.o: t2lp.c interface.h
#	$(CC) $(CFLAGS) -c t2lp.c

