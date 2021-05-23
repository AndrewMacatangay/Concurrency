all: SimpleThreads Join Detach Ownership Guard Vector

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

Ownership.o: Ownership.cpp
	g++ -c -Wall Ownership.cpp

Guard: Guard.o
	g++ -o Guard Guard.o -pthread
	rm Guard.o

Guard.o: Guard.cpp
	g++ -c -Wall Guard.cpp

Vector: Vector.o
	g++ -o Vector Vector.o -pthread
	rm Vector.o

Vector.o: Vector.cpp
	g++ -c -Wall Vector.cpp

clean:
	rm SimpleThreads Join Detach Ownership Guard Vector *.o
