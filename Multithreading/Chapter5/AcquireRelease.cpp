//This program is a lock-free implementation of code that will communicate
//the a producer-vendor relationship by using memory ordering. We can set up
//our scene in a fast-foot kitchen where a countertop can hold one burger only
//and only one burger can be made at a time

#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

//Countertop will be a shared variable between the producer and vendor
//and will be atomized to avoid any unexpected read/write behaviors
atomic<bool> countertop;

//Ready will be the actual status of if a burger is available
bool ready;

//Using "memory_order_release", "ready" will be set to true before the
//call to .store() which means that the burger MUST be made and ready
//the burger can be put on the countertop for the vendor to see
void makeBurger()
{
	//The burger is made
	ready = true;
	cout << "Burger made!" << endl;

	//Place the burger on the countertop so that the vendor can take over
	countertop.store(ready, memory_order_release);
}

//Using "memory_order_acquire", "ready" will not be set to false before
//the call to .load() which means that the burger must be sold to a
//customer first before the vendor can take the burger from the countertop
void sellBurger()
{
	//Wait until the burger is available on the countertop
	while(!countertop.load(memory_order_acquire));

	//The burger is sold
	ready = false;
	cout << "Burger sold!" << endl;

	//Take the burger from the countertop so that the producer can take over
	countertop.store(ready, memory_order_release);
}

int main()
{
	//There is no burger on the countertop and no burger has been made
	countertop = false;
	ready = false;

	thread t1(makeBurger);
	thread t2(sellBurger);

	t1.join();
	t2.join();

	//The countertop is not lock-free. The producer and the vendor both have access
	//to the countertop at all times and accessing the countertop is not mutually
	//exclusive to both parties
	cout << (countertop.is_lock_free() ? "Lock-free!" : "Not lock-free...") << endl;
	
	return 0;
}
