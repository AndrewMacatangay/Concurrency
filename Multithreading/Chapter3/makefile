all: Race SimpleMutex LockRace

Race:
	g++ -g -Wall -o Race Race.cpp -pthread

SimpleMutex:
	g++ -g -Wall -o SimpleMutex SimpleMutex.cpp -pthread

LockRace:
	g++ -g -Wall -o LockRace LockRace.cpp -pthread

clean:
	rm Race SimpleMutex LockRace
