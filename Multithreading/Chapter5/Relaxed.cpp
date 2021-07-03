//Single threaded programs are required to follow the as-if rule. The observed ordering
//should match the program order. In multi-threaded programs, the memory ordering is
//decided for the programmer. The observed ordering may be different than the program
//order. When using memory_order_relaxed, memory accesses have no guarantee of following
//a particular order. The only thing that is guaranteed is that the option will be
//executed at some point.

#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

atomic<int> x, y;
int r1, r2;

//Since the memory order is relaxed, any of the operations in func1() and func2()
//can be run in any order. We would assume think that the natural order would be 
//A, B, C, then D since thread t1 is called before t2. However, these threads run
//concurrently. It is possible for t2 to run before t1 and so we can have the
//order C, D, A, B, leaving the values to be r1 = 42, x = 42, r2 = 0, and y = 42.
//In fact since any re-ordering can happen, D, B, A, C is possible which would
//set all values to 42. A relaxed ordering is commonly used in incrementing some
//counter since it may not need to be synced with other operations.

void func1()
{
	r1 = y.load(memory_order_relaxed);	//A
	x.store(r1, memory_order_relaxed);	//B
}

void func2()
{
	r2 = x.load(memory_order_relaxed);	//C
	y.store(42, memory_order_relaxed);	//D
}

int main()
{
	//Set both values to 0 before we change them
	x = y = 0;

	//Launch 2 threads. Both threads will run their course without synchronization
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();

	cout << "R1: " << r1 << " R2: " << r2 << " x: " << x << " y: " << y << endl;

	return 0;
}
