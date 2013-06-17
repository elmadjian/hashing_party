#=================================================
#|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||
#||||| Nome: Carlos Eduardo Leao Elmadjian |||||||
#||||| NUSP: 5685741 |||||||||||||||||||||||||||||
#||||| Arquivo: makefile |||||||||||||||||||||||||
#=================================================
CFLAGS=-Wall -ansi -pedantic
CC=gcc
#=================================================
#
ep4encenc: main.o t1enc.o t2enc.o t_aux.o
	$(CC) $(CFLAGS) t1enc.o t2enc.o t_aux.o main.o -o ep4encenc 

ep4enclp: main.o t1enc.o t2lp.o t_aux.o
	$(CC) $(CFLAGS) t1enc.o t2lp.o t_aux.o main.o -o ep4enclp 

ep4lpenc: main.o t1lp.o t2enc.o t_aux.o
	$(CC) $(CFLAGS) t1lp.o t2enc.o t_aux.o main.o -o ep4lpenc

ep4lplp: main.o t1lp.o t2lp.o t_aux.o
	$(CC) $(CFLAGS) t1lp.o t2lp.o t_aux.o main.o -o ep4lplp

main.o: main.c interface.h
	$(CC) $(CFLAGS) -c main.c

t_aux.o: t_aux.c interface.h
	$(CC) $(CFLAGS) -c t_aux.c

t1enc.o: t1enc.c interface.h
	$(CC) $(CFLAGS) -c t1enc.c

t2enc.o: t2enc.c interface.h
	$(CC) $(CFLAGS) -c t2enc.c

t1lp.o: t1lp.c interface.h
	$(CC) $(CFLAGS) -c t1lp.c

t2lp.o: t2lp.c interface.h
	$(CC) $(CFLAGS) -c t2lp.c

clean:
	rm *.o
