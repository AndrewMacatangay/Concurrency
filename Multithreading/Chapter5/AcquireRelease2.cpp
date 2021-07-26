/*
In this program, we demonstrate acquire-release barriers by having a thread that deals with
writes and another thread that deals with reads. The writing thread will has a release barrier
and all prior writes to memory will be executed. In the reading thread, the acquire will sync
with the release and all read/writes will happen after the acquire barrier.

Sidenote: Think of barriers as locks. Operations can be reordered to be under an acquire barrier
and above a release barrier.
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

atomic<bool> x, y;
atomic<int> z;

void writeXThenY()
{
	x.store(true, memory_order_relaxed);
	y.store(true, memory_order_release);
}

void readYThenX()
{
	while(!y.load(memory_order_acquire));
	if (x.load(memory_order_relaxed))
		z++;
}

int main()
{
	x = y = false;
	z = 0;

	thread a(writeXThenY);
	thread b(readYThenX);

	a.join();
	b.join();

	cout << "Z: " << z << endl;

	return 0;
}
