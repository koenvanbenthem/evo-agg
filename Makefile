CFLAGS = -O -std=c++11
CC = g++

main: main.o mod.o svgplot.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

mod.o: mod.cpp
	$(CC) $(CFLAGS) -c mod.cpp

svgplot.o: svgplot.cpp
	$(CC) $(CFLAGS) -c svgplot.cpp

clean:
	rm -f core *.o
