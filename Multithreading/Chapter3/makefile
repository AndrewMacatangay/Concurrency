all: Race SimpleMutex LockRace Protect

Race:
	g++ -g -Wall -o Race Race.cpp -pthread

SimpleMutex:
	g++ -g -Wall -o SimpleMutex SimpleMutex.cpp -pthread

LockRace:
	g++ -g -Wall -o LockRace LockRace.cpp -pthread

Protect:
	g++ -g -Wall -o Protect Protect.cpp -pthread

clean:
	rm Race SimpleMutex LockRace Protect
