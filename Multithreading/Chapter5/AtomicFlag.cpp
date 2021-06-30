//The purpose of this project is to emulate a spin-lock using
//an atomic flag. Atomic flags are lock-free and will avoid
//deadlocks and/or other undefined behaviors. Below, each
//thread will first check if the flag is set. If it is, the
//thread will retry to see if the flag has been cleared. If
//the flag is not set, the thread will set the flag, perform
//the code in the critical section, and then the flag should
//be cleared once the critical section is executed. An interesting
//thing to note is that the main thread could finish before all
//of the detached threads have finished executing, so we can
//make a while loop to wait for all threads to finish.

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
using namespace std;

atomic_flag flag = ATOMIC_FLAG_INIT;
int count = 0;

void print(int threadNumber)
{	
	while(flag.test_and_set());
	
	cout << "Thread Number: " << threadNumber << endl;
	count++;
	
	flag.clear();
}

int main()
{
	for (int x = 0; x < 10; x++)
		thread(print, x).detach();

	//Wait for all 10 threads to finish. Sleeping in between
	//each check to the while condition will free up the CPU
	//from the main thread so that it can work on other tasks
	while(count ^ 10)
		this_thread::sleep_for(1ms);

	return 0;
}
