CC = g++
CFLAGS = -Wall -O3 --std=c++11

08.1.exe : 08.1.o random.o
	$(CC) random.o 08.1.o -o 08.1.exe -no-pie

08.1.o : 08.1.cpp
	$(CC) -c 08.1.cpp -o 08.1.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 08.1.exe seed.out