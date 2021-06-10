//This program demonstrates the use of shared_futures. Shared futures are useful
//for broadcasting purposes. You could send a message concurrently so that each
//thread recieves the messages almost simultaneously or in some order (queued).
//A promise will be created by the broadcaster to the "listening" threads. Once
//the broadcaster sets the value for the promise, the promise is fulfilled, and
//the listening threads will recieve the message.
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <future>
using namespace std;

static int ID = 0;
mutex m;

void recieve(shared_future<string> sf)
{
	//Since futures and promises are not immune to race conditions, we need
	//to have a lock_guard or unique_lock here to syncrhonize the threads
	lock_guard<mutex> l(m);

	ID++;
	string message = sf.get();

	cout << "Thread " << ID << " recieves the message '" << message << "'" << endl;
}

int main()
{
	//Create a promise with an associated future to be shared
	//between multiple threads of execution
	promise<string> p;
	future<string> f = p.get_future();
	shared_future<string> sf = f.share();

	//Launch 5 threads that will execute recieve() which takes a
	//shared_future as an argument. Each of these futures are stored
	//in a vector
	vector<future<void>> futures;
	for (int x = 0; x++ < 5; futures.push_back(async(recieve, sf)));

	//Broadcast the message here
	p.set_value("hello");

	return 0;
}
