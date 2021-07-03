all: AtomicFlag AtomicBool Relaxed

AtomicFlag: AtomicFlag.cpp
	g++ -g -Wall AtomicFlag.cpp -o AtomicFlag -pthread

AtomicBool: AtomicBool.cpp
	g++ -g -Wall AtomicBool.cpp -o AtomicBool -pthread

Relaxed: Relaxed.cpp
	g++ -g -Wall Relaxed.cpp -o Relaxed -pthread

clean:
	rm AtomicFlag AtomicBool Relaxed
