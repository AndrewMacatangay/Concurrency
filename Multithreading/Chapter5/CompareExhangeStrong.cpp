#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

atomic_flag af = ATOMIC_FLAG_INIT;
atomic<int> a;

void func1(int x)
{
	int temp = x;
	while(!a.compare_exchange_strong(x, x + 1))
		x = temp;

	//a.compare_exchange_strong(x, x + 1, memory_order_acq_rel);
	//cout << "Thread " << a.load(memory_order_acquire) << " has been processed!" << endl;

	while(af.test_and_set());
	cout << "Thread " << x << " has been processed!" << endl;
	af.clear();
	x++;
	//a.store(x, memory_order_release);
	a.store(x);

}

int main()
{
	a = 0;
	vector<thread> threads;

	for(int x = 0; x < 10; x++)
		threads.push_back(thread(func1, x));

	for(thread& t : threads)
		t.join();

	cout << (a.is_lock_free() ? "Lock-Free!" : "Not Lock-Free...") << endl;

	return 0;
}
