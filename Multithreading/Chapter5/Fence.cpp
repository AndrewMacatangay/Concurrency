/*
Fences are another type of memory barrier that allow the use to synchornize
instructions. Instead of using a variable to synychonize instructions, a fence
can be used instead. This is useful when you don't want to modify some atomic
variable.
*/

#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

atomic<bool> x, y;
atomic<int> z;

//x will store true, and once the fence indicates the release
//semantics, x's store should be seen after the acquire fence
//in the next function
void writeXThenY()
{
	x.store(1, memory_order_relaxed);
	atomic_thread_fence(memory_order_release);
	y.store(1, memory_order_relaxed);
}

//y will wait until it is set to true. After the acquire fence the
//condition will be true since x has been set to true before the release
//and loaded after the acquire. Therefore, z is incremented
void readYThenX()
{
	while(!y.load(memory_order_relaxed));
	atomic_thread_fence(memory_order_acquire);
	if (x.load(memory_order_relaxed))
		z++;
}

int main()
{
	thread t1(writeXThenY);
	thread t2(readYThenX);

	t1.join();
	t2.join();

	cout << z << endl;
	return 0;
}
