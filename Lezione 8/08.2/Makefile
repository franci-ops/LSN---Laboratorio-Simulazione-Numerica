CC = g++
CFLAGS = -Wall -O3 --std=c++11

08.2.exe : 08.2.o random.o
	$(CC) random.o 08.2.o -o 08.2.exe -no-pie

08.2.o : 08.2.cpp
	$(CC) -c 08.2.cpp -o 08.2.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 08.2.exe seed.out