#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
using namespace std;

//Network connection refreshes every 1s.

timed_mutex m;

int func(int threadNumber)
{
	unique_lock<timed_mutex> l(m, defer_lock);
	while(!l.try_lock_for(chrono::milliseconds(200)))
	{
		cout << "Waiting..." << endl;
	}
	this_thread::sleep_for(1s);
	return threadNumber;
}


int main()
{
	vector<future<int>> futures(4);
	for (int x = 0; x < 4; x++)
	{
		futures[x] = async(launch::async, func, x);
		//cout << f.get() << endl;
	}

	for (int x = 0; x < 4; x++)
		cout << futures[x].get() << endl;

	return 0;
}
