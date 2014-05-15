all: main

main: main.o executor.o Pharser.o
	g++ -main.o executor.o Pharser.o -o main
	
main.o:
	g++ -c main.cpp

executor.o:
	g++ -c executor.cpp

Pharser.o:
	g++ -c Pharser.cpp

clean:
	rm -rf *o main
