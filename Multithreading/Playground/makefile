Alphabet: Alphabet.o
	g++ -o Alphabet Alphabet.o -pthread
	rm Alphabet.o

Alphabet.o: Alphabet.cpp
	g++ -c -Wall Alphabet.cpp

Chapter2: Chapter2.o
	g++ -o Chapter2 Chapter2.o -pthread
	rm Chapter2.o

Chapter2.o: Chapter2.cpp
	g++ -c -Wall Chapter2.cpp

clean:
	rm Alphabet Chapter2
