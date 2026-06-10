CC = g++
CFLAGS = -Wall -O3 --std=c++11

02.1.exe : 02.1.o random.o
	$(CC) random.o 02.1.o -o 02.1.exe -no-pie

02.1.o : 02.1.cpp
	$(CC) -c 02.1.cpp -o 02.1.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 02.1.exe seed.out