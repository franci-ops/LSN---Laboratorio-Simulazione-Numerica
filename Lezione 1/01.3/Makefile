CC = g++
CFLAGS = -Wall -O3 --std=c++11

01.3.exe : 01.3.o random.o
	$(CC) random.o 01.3.o -o 01.3.exe -no-pie

01.3.o : 01.3.cpp
	$(CC) -c 01.3.cpp -o 01.3.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 01.3.exe seed.out