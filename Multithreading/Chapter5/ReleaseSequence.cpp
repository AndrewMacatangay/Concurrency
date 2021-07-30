/*
The purpose of this program is to synchronize release operations with acquire operations.
The thread that runs produce() will produce some amount of items that can be consumed by
consumer threads. Each consumemr thread will consume one and only one element. Each 
release/acquire is synchonized with each other such that no data race will occur.
*/

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> remainingSupply(0);
vector<int> v;

void produce(int supply)
{
	v = vector<int>(supply, 0);

	//Send the supply count
	remainingSupply.store(supply, memory_order_release);	
}

void consume(int supply, int threadNumber)
{
	//If the threadNumber exceeds the supply, then we cannot
	//consume an element
	if (threadNumber > supply)
		return;

	int index;

	//Wait until the release signal has been sent
	while((index = remainingSupply.fetch_sub(1, memory_order_acquire)) <= 0);
	
	//"Consuming" the element is just updating the array
	v[supply - index] = threadNumber;
}

int main()
{
	vector<thread> consumers;

	//This code will work with uneven supply/demand
	int supply = 5;
	int demand = 5;

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
