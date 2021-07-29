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

void consume(int threadNumber)
{
	int index = 0;
	while((index = counter.fetch_sub(1, memory_order_acquire)) <= 0);
	v[index - 1]++;
	//v.push_back(1);
}

int main()
{
	vector<thread> consumers;

	thread t1(produce, 7);
	
	for (int x = 0; x < 6; x++)
		consumers.push_back(thread(consume, x + 1));

	t1.join();

	for (thread& t : consumers)
		t.join();

	for(size_t x = 0; x < v.size(); x++)
		cout << "Thread " << x + 1 << ": " << v[x] << endl;

	return 0;
}
