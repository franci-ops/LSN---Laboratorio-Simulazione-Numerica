CC = g++
CFLAGS = -Wall -O3 --std=c++11

03.1.exe : 03.1.o random.o
	$(CC) random.o 03.1.o -o 03.1.exe -no-pie

03.1.o : 03.1.cpp
	$(CC) -c 03.1.cpp -o 03.1.o $(CFLAGS)

random.o : ../../RANDOM/random.cpp ../../RANDOM/random.h
	$(CC) -c ../../RANDOM/random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o 03.1.exe seed.out