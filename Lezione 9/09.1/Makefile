CC = g++

CFLAGS = -O3 --std=c++11

all: main.exe

main.exe : main.o città.o percorso.o popolazione.o random.o ga.o
	$(CC) main.o città.o percorso.o popolazione.o random.o ga.o -o main.exe -no-pie

main.o : main.cpp città.h percorso.h popolazione.h ga.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

città.o : città.cpp città.h
	$(CC) -c città.cpp -o città.o $(CFLAGS)

percorso.o : percorso.cpp percorso.h città.h
	$(CC) -c percorso.cpp -o percorso.o $(CFLAGS)

popolazione.o : popolazione.cpp popolazione.h percorso.h
	$(CC) -c popolazione.cpp -o popolazione.o $(CFLAGS)

ga.o : ga.cpp ga.h popolazione.h percorso.h ../../RANDOM/random.h
	$(CC) -c ga.cpp -o ga.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)
clean :
	rm -f *.o main.exe