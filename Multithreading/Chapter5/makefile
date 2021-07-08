all: AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer AtomicUDT

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

AtomicPointer: AtomicPointer.cpp
	g++ -g -Wall AtomicPointer.cpp -o AtomicPointer

AtomicUDT: AtomicUDT.cpp
	g++ -g -Wall AtomicUDT.cpp -o AtomicUDT -latomic

clean:
	rm AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer AtomicUDT
