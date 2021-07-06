/*
The exchange function is an atomic read-modify-write operation. The
programmer may implement a strong or week compare_exchange. Machines
may be susceptible to sporadic failures, so it is common to put
compare_exchange_weak() within a while loop:
	while (!b.compare_exchange_weak(expected, true) && !expected);

Using compare_exchange_strong needs more overhead, but the library will
take care of any failures. In pseudo-code the hardware implementation
may act as follows:

bool compare_exchange_strong(T& expected, T newValue)
{
	lock_guard<mutex> l(m);		//Where "m" is defined as: mutex m;
	T temp = atomicValue;

	if (temp != expected)
	{
		expected = temp;
		return 0;
	}

	atomicValue = newValue;
	return 1;
}
*/

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

atomic_flag af = ATOMIC_FLAG_INIT;
atomic<int> a;

void func(int x)
{
	int temp = x;

	//If the compare fails, "x" acquires the value of "a".
	//For the sake of this program, we will change "x" to be
	//our initial value from the line above
	while(!a.compare_exchange_strong(x, x + 1))
		x = temp;

	//This spinlock using an atomic_flag will deter any race
	//condition on "cout"
	while(af.test_and_set());
	cout << "Thread " << x << " has been processed!" << endl;
	af.clear();
	
	//Increment x and store it into "a" so that the next thread
	//can process it
	a.store(++x);
}

int main()
{
	a = 0;
	vector<thread> threads;

	for(int x = 0; x < 10; x++)
		threads.push_back(thread(func, x));

	for(thread& t : threads)
		t.join();

	cout << (a.is_lock_free() ? "Lock-Free!" : "Not Lock-Free...") << endl;

	return 0;
}
