build: HelloWorld.o
	g++ -g -Wall -o HelloWorld HelloWorld.o -pthread
	rm HelloWorld.o

run:
	./HelloWorld

HelloWorld.o: HelloWorld.cpp
	g++ -c HelloWorld.cpp
