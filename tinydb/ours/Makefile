all: main

main: main.o
	g++ main.o
	
fail.o: fail.cpp
	g++ -c fail.cpp
	
main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -rf *o main
