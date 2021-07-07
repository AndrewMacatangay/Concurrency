/*
This program implements a spin-lock using an atomic<bool>. Note that this program is
not lock free since one thread can block another from accessing a shared resource
(the shared resource being std::cout in this exmaple). Instead of using an atomic
flag and using test_and_set(), we use an atomic<bool> and compare_and_exchange()
to emulate a similar behavior. 
*/

//"Everyone and everything is lock-free until dependencies or shared resources are bound to them"

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

class Spinlock
{
	private:
		atomic<bool> a;
	public:
		Spinlock() : a(0) { }

		//Attempt to lock by comparing the atomic to the expected value. If the atomic is false,
		//we set it to true and continue to the critical section. If the atomic is true, then
		//we continue to spin
		void lock()
		{	
			bool expected = 0;
			while(!a.compare_exchange_strong(expected, 1, memory_order_acquire))
				expected = 0;
		}

		//We have finished executing the critical section, so we reset the bool
		void unlock()
		{
			a.store(0, memory_order_release);
		}

		//This checks if the atomic is lock free, not the spinlock itself
		bool is_lock_free()
		{
			return a.is_lock_free();
		}
};

//Spinlock instance to guard the cout statement in func() from
//race conditions
Spinlock spinlock;

void func(int x)
{
	spinlock.lock();
	cout << "Thread " << x << " processed!" << endl;
	spinlock.unlock();
}

int main()
{
	vector<thread> threads;

	for (int x = 0; x < 10; x++)
		threads.push_back(thread(func, x));

	for (thread& t : threads)
		t.join();

	//Important note: The atomic inside the spinlock is lock-free, but the
	//spinlock itself is not lock-free
	cout << (spinlock.is_lock_free() ? "Lock-Free!" : "Not Lock-Free...") << endl;

	return 0;
}
