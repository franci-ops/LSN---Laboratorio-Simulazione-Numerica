CC = g++
CFLAGS = -Wall -O3 --std=c++11

01.1.exe : 01.1.o random.o
	$(CC) random.o 01.1.o -o 01.1.exe -no-pie

01.1.o : 01.1.cpp
	$(CC) -c 01.1.cpp -o 01.1.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 01.1.exe seed.out