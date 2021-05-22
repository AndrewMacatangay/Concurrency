all: SimpleThreads Join Detach Ownership

SimpleThreads: SimpleThreads.o
	g++ -o SimpleThreads SimpleThreads.o -pthread
	rm SimpleThreads.o

SimpleThreads.o: SimpleThreads.cpp
	g++ -c -Wall SimpleThreads.cpp

Join: Join.o
	g++ -o Join Join.o -pthread
	rm Join.o

Join.o: Join.cpp
	g++ -c -Wall Join.cpp

Detach: Detach.o
	g++ -o Detach Detach.o -pthread
	rm Detach.o

Detach.o: Detach.cpp
	g++ -c -Wall Detach.cpp

Ownership: Ownership.o
	g++ -o Ownership Ownership.o -pthread
	rm Ownership.o

Ownership.o:
	g++ -c -Wall Ownership.cpp

clean:
	rm SimpleThreads Join Detach Ownership *.o
