#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

//Start with some X amount of stock, and decrement it as more burgers
//are produced. If the consumer is not able to recieve a burger because
//of the lack of stock, notify the consumer.
atomic<int> ingredientSet(3), buns(0), patty(0), cheese(0), burgers(0), isReady(0);

//Get rid of a set of ingredients. If there is still more, then return the order number,
//otherwise 0
void produce(int orderNumber)
{
	int store = ingredientSet.fetch_sub(1, memory_order_relaxed);
	isReady.store(store ? orderNumber : -orderNumber, memory_order_release);
}

//Let the cashier wait until its ready. If they waited too long, there are no more
//sets of ingredients
void consume(int orderNumber)
{
	//Race conditions with "cycles?"
	//int cycles = 0;

	//while(cycles != 150000)
	int ready = isReady.load(memory_order_acquire);
	for(; ready != orderNumber && ready != -orderNumber; ready = isReady.load(memory_order_acquire));

	if (ready == orderNumber)
		burgers.fetch_add(1);
	else if (ready == -orderNumber)
		cout << "tset" << endl;
}

int main()
{
	vector<thread> consumers;
	vector<thread> producers;

	for (int x = 1; x <= 2; x++)
		consumers.push_back(thread(consume, x));

	for (int x = 1; x <= 2; x++)
		producers.push_back(thread(produce, x));

	for (thread& t : consumers)
		t.join();
	for (thread& t : producers)
		t.join();

	cout << "Burgers: "  << burgers << endl;
	//cout << buns << " " << patty << " " << cheese << endl;
	//cout << ingredientSet << endl;
	return 0;
}
