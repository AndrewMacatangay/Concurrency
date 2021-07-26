all: AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer AtomicUDT FreeFunction SequentiallyConsistent Relaxed2 AcquireRelease2

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

FreeFunction: FreeFunction.cpp
	g++ -g -Wall FreeFunction.cpp -o FreeFunction

SequentiallyConsistent: SequentiallyConsistent.cpp
	g++ -g -Wall SequentiallyConsistent.cpp -o SequentiallyConsistent -pthread

Relaxed2: Relaxed2.cpp
	g++ -g -Wall Relaxed2.cpp -o Relaxed2 -pthread

AcquireRelease2: AcquireRelease2.cpp
	g++ -g -Wall AcquireRelease2.cpp -o AcquireRelease2 -pthread

clean:
	rm AtomicFlag AtomicBool Relaxed AcquireRelease CompareExchange AtomicPointer AtomicUDT FreeFunction SequentiallyConsistent Relaxed2 AcquireRelease2
