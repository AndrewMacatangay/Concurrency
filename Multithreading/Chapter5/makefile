all: AtomicFlag AtomicBool

AtomicFlag: AtomicFlag.cpp
	g++ -g -Wall AtomicFlag.cpp -o AtomicFlag -pthread

AtomicBool: AtomicBool.cpp
	g++ -g -Wall AtomicBool.cpp -o AtomicBool -pthread

clean:
	rm AtomicFlag AtomicBool
