all: Race SimpleMutex LockRace Protect SimpleDeadlock ExchangeDeadlock FineGrain CoarseGrain OptimizedFineGrain LazyInitialization RecursiveLock

Race:
	g++ -g -Wall -o Race Race.cpp -pthread

SimpleMutex:
	g++ -g -Wall -o SimpleMutex SimpleMutex.cpp -pthread

LockRace:
	g++ -g -Wall -o LockRace LockRace.cpp -pthread

Protect:
	g++ -g -Wall -o Protect Protect.cpp -pthread

SimpleDeadlock:
	g++ -g -Wall -o SimpleDeadlock SimpleDeadlock.cpp -pthread

ExchangeDeadlock:
	g++ -g -Wall -o ExchangeDeadlock ExchangeDeadlock.cpp -pthread

FineGrain:
	g++ -g -Wall -o FineGrain FineGrain.cpp -pthread

CoarseGrain:
	g++ -g -Wall -o CoarseGrain CoarseGrain.cpp -pthread

OptimizedFineGrain:
	g++ -g -Wall -o OptimizedFineGrain OptimizedFineGrain.cpp -pthread

LazyInitialization:
	g++ -g -Wall -o LazyInitialization LazyInitialization.cpp -pthread

RecursiveLock:
	g++ -g -Wall -o RecursiveLock RecursiveLock.cpp -pthread

clean:
	rm Race SimpleMutex LockRace Protect SimpleDeadlock ExchangeDeadlock FineGrain CoarseGrain OptimizedFineGrain LazyInitialization RecursiveLock
