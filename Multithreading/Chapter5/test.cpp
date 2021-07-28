#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

atomic<int> a(0);
atomic<int> b(0);
atomic<int> burgers(0);
atomic<int> go(0);

//Add updates
void func1(int val, int batchNumber)
{
	int expected = batchNumber;
	while(!go.compare_exchange_strong(expected, 0, memory_order_acq_rel))
		expected = batchNumber;
	//a.store(val, memory_order_relaxed);
	b.store(batchNumber, memory_order_release);
}

//Package updates
void func2(int batchNumber)
{
	go.store(batchNumber, memory_order_relaxed);
	int expected = batchNumber;
	while(!b.compare_exchange_strong(expected, 0, memory_order_acq_rel))
		expected = batchNumber;
}

//Download updates
void func3()
{
	while(b.load(memory_order_acquire) != 0);
	//cout << "A: " << a.load(memory_order_relaxed) << endl;
	burgers++;
}

int main()
{
	vector<thread> orders;
	vector<thread> status;
	vector<thread> success;

	for(int x = 1; x <= 100; x++)
	{
		orders.push_back(thread(func1, x * 10, x));
		status.push_back(thread(func2, x));
		success.push_back(thread(func3));
	}
	
/*	for(int x = 1; x <= 100; x++)
		status.push_back(thread(func2, x));

	for(int x = 1; x <= 100; x++)
		success.push_back(thread(func3));
*/
	for(thread& t : orders)
		t.join();

	for(thread& t : status)
		t.join();

	for(thread& t : success)
		t.join();

	/*thread t1(func1, 10, 1);
	thread t2(func2, 1);
	thread t3(func3);
	thread t4(func1, 20, 2);
	thread t5(func2, 2);
	thread t6(func3);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();*/
	
	cout << "Burgers: " << burgers << endl;

	return 0;
}
