CC = g++
CFLAGS = -Wall -O3 --std=c++11

02.2.exe : 02.2.o random.o
	$(CC) random.o 02.2.o -o 02.2.exe -no-pie

02.2.o : 02.2.cpp
	$(CC) -c 02.2.cpp -o 02.2.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 02.2.exe seed.out