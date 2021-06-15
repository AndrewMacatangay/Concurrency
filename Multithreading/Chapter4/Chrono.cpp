//This program is meant to demonstrate how we can use the chrono library
//to set durations on wait/sleep/try "for" functions on objects in the
//future library. Note that time_points can be used for wait/sleep/try
//"until" functions on objects in the future library. Timed mutexes are
//introduced in this example.

//The inspiration for this program is based on some MMORPG game where 
//players share certain game objects. In this example, each player (thread)
//will attempt to pick a lock on a shared chest that contains loot. Each
//thread will attempt to lock-pick every 0.4 seconds for simplicity. Once
//opened, the chest will be available for 1 second for the respective thread
//to loot. No other thread can open the cheat at this point. The program
//finishes when all threads have joined and the results for each thread are
//printed.

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
using namespace std;

//Mutexes needed for locking. This is the first example using
//a timed mutex
timed_mutex m1;
mutex m2;

bool lootable = 1;

//This function will simulate a player in a game to loot a chest
//being able to attempt a lock-pick every 0.4 seconds. If they are
//successful, it will take 1 second to loot the chest, and no other
//player will be able to open the chest while another player is looting
int func(int threadNumber)
{
	//The first lock will be used to attempt to pick the lock
	//and the second lock will be used to synchonize the printing
	//statements so that there is no data race on std::cout
	unique_lock<timed_mutex> l(m1, defer_lock);
	unique_lock<mutex> l2(m2, defer_lock);

	int attempts = 0;

	//Every 0.4 seconds, a thread will attempt to pick the lock
	//When successful, a message will be printed to the console
	while(!l.try_lock_for(chrono::milliseconds(400)) && !lootable)
	{
		l2.lock();
		cout << "Player " << threadNumber << " attempts to pick the lock and fails...\n";
		l2.unlock();
		attempts++;
	}

	//The player opens the chest, but it takes a second to loot the chest
	cout << "Player " << threadNumber << " opens the chest!\n";
	lootable = !lootable;
	this_thread::sleep_for(1s);
	lootable = !lootable;

	//The player succeeds, and the number of attempts is returned
	l2.lock();
	cout << "Player " << threadNumber << " successfully loots the chest!\n"; 
	l2.unlock();

	//We add one for the successful attempt
	return attempts + 1;
}

int main()
{
	//Each future will retrieve the number of attempts each
	//thread has tried. The results will be stored in the
	//results vector so that we can join the threads before
	//printing out the results. Otherwise, we would have a
	//race condition
	vector<future<int>> futures(4);
	vector<int> results(4);

	//Launch 4 threads
	for (int x = 0; x < 4; x++)
		futures[x] = async(launch::async, func, x + 1);

	//Join 4 threads and save the results in the results vector
	for(int x = 0; x < 4; x++)
		results[x] = futures[x].get();

	//This cout must be here rather than before the last for-loop
	//since we want to separate the line after all threads have joined.
	//We would have a race condition on std::cout otherwise
	cout << endl;

	//Print out the results
	for (int x = 0; x < 4; x++)
		cout << "Player " << x + 1 << " attempted " << results[x] << " lock-pick(s) before success" << endl;

	return 0;
}
