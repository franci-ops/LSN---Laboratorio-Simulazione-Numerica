CC = g++
CFLAGS = -Wall -O3 --std=c++11

01.2.exe : 01.2.o random.o
	$(CC) random.o 01.2.o -o 01.2.exe -no-pie

01.2.o : 01.2.cpp
	$(CC) -c 01.2.cpp -o 01.2.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 01.2.exe seed.out