all: Race SimpleMutex LockRace

Race: Race.cpp
	g++ -g -Wall -o Race Race.cpp -pthread

SimpleMutex: SimpleMutex.cpp
	g++ -g -Wall -o SimpleMutex SimpleMutex.cpp -pthread

LockRace: LockRace.cpp
	g++ -g -Wall -o LockRace LockRace.cpp -pthread

clean:
	rm Race SimpleMutex LockRace
