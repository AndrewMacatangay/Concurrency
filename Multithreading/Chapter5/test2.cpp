#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> counter(0);
vector<int> v;

void produce(int stock)
{
	v = vector<int>(stock, 0);
	counter.store(stock, memory_order_release);	
}

void consume(int stock, int threadNumber)
{
	int index = 0;
	while((index = counter.fetch_sub(1, memory_order_acquire)) <= 0)
	{
		if (index < 0)
			return;
		//cout << stock << " " << threadNumber << " " << index << endl;
	};
	v[stock - index] = threadNumber;
}

int main()
{
	vector<thread> consumers;
	int supply = 5;
	int demand = 6;

	thread t1(produce, supply);
	
	for (int x = 0; x < demand; x++)
		consumers.push_back(thread(consume, supply, x + 1));

	t1.join();

	for (thread& t : consumers)
		t.join();

	for(int x = 0; x < (demand < supply ? demand : supply); x++)
		cout << "Thread " << x + 1 << ": " << v[x] << endl;

	return 0;
}
