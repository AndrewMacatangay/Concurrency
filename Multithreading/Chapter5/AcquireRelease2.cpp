#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

//Start with some X amount of stock, and decrement it as more burgers
//are produced. If the consumer is not able to recieve a burger because
//of the lack of stock, notify the consumer.
atomic<int> ingredientSet(3), buns(0), patty(0), cheese(0), burgers(0), isReady(-1);

void produce(int orderNumber)
{
	//buns.fetch_add(1, memory_order_relaxed);
	//patty.fetch_add(1, memory_order_relaxed);
	//cheese.fetch_add(1, memory_order_relaxed);

	int stock = ingredientSet.load(memory_order_seq_cst);
	if(stock)
		ingredientSet.fetch_sub(1, memory_order_seq_cst);
	isReady.store(stock ? orderNumber : -1, memory_order_release);
}

void consume(int orderNumber)
{
	int cycles = 0;
	while(cycles < 150000 && isReady.load(memory_order_acquire) != orderNumber)
		cycles++;
	/*if (cycles != 100000 && 
	    buns.load(memory_order_relaxed) &&
	    patty.load(memory_order_relaxed) &&
	    cheese.load(memory_order_relaxed))*/
	    if (cycles != 150000)
	    {
		//cout << "Success!" << " " << cycles << endl;
		//buns.fetch_sub(1, memory_order_relaxed);
		//patty.fetch_sub(1, memory_order_relaxed);
		//cheese.fetch_sub(1, memory_order_relaxed);
		burgers.fetch_add(1, memory_order_relaxed);
	    }
	else
		cout << "Failure..." << " " << orderNumber << endl;
}

int main()
{
	vector<thread> consumers;
	vector<thread> producers;

	for (int x = 0; x < 3; x++)
		consumers.push_back(thread(consume, x));

	for (int x = 0; x < 3; x++)
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
