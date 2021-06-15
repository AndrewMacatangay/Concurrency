#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
using namespace std;

//Network connection refreshes every 1s.

timed_mutex m;
mutex m2;

int func(int threadNumber)
{
	unique_lock<timed_mutex> l(m, defer_lock);
	unique_lock<mutex> l2(m2, defer_lock);

	while(!l.try_lock_for(chrono::milliseconds(400)))
	{
		l2.lock();
		cout << "Player " << threadNumber << " attempts to pick the lock and fails...\n";
		l2.unlock();
	}

	cout << "Player " << threadNumber << " opens the chest!\n";
	this_thread::sleep_for(1s);

	l2.lock();
	cout << "Player " << threadNumber << " successfully loots the chest!\n"; 
	l2.unlock();

	return threadNumber;
}


int main()
{
	vector<future<int>> futures(4);
	
	for (int x = 0; x < 4; x++)
		futures[x] = async(launch::async, func, x + 1);

	for (int x = 0; x < 4; x++)
		futures[x].get();

	return 0;
}
