all: AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer

AtomicFlag: AtomicFlag.cpp
	g++ -g -Wall AtomicFlag.cpp -o AtomicFlag -pthread

AtomicBool: AtomicBool.cpp
	g++ -g -Wall AtomicBool.cpp -o AtomicBool -pthread

Relaxed: Relaxed.cpp
	g++ -g -Wall Relaxed.cpp -o Relaxed -pthread

AcquireRelease: AcquireRelease.cpp
	g++ -g -Wall AcquireRelease.cpp -o AcquireRelease -pthread

CompareExchange: CompareExchange.cpp
	g++ -g -Wall CompareExchange.cpp -o CompareExchange -pthread

AtomicPointer:
	g++ -g -Wall AtomicPointer.cpp -o AtomicPointer -pthread

clean:
	rm AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer
