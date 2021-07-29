#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> counter(0), ingredients(3);
vector<int> v;

void produce()
{
	int numElements = ingredients.load(memory_order_relaxed);
	v.clear();
	for(int x = 0; x < numElements; x++)
		v.push_back(x * 10);
	counter.store(numElements, memory_order_release);
	
}

void consume()
{
	int index = 0;
	while(1)
	{
		if((index = counter.fetch_sub(1, memory_order_acquire)) <= 0)
		{
			if (index < 0)
				break;
			continue;
		}
		else
		{
			cout << v[index - 1] << endl;
			break;
		}
	}
}

int main()
{
	vector<thread> consumers;

	thread t1(produce);

	for (int x = 0; x < 5; x++)
		consumers.push_back(thread(consume));

	t1.join();

	for (thread& t : consumers)
		t.join();

	return 0;
}
