//This program demonstrates a simple implementation of a
//race condition. Since both threads use "cout" as a 
//shared resource, and both threads are not mutually
//exclusive (both threads can use "cout" at the same time),
//then a race condition occurs. The output may result in
//some underined behavior.

#include <iostream>
#include <thread>
using namespace std;

void func()
{
	for (int x = 0; x < 100; x += 2)
		cout << "In func: " << x << endl;
}

int main()
{
	thread t (func);
	
	for (int x = 1; x < 100; x += 2)
		cout << "In main: " << x << endl;

	t.join();
	return 0;
}
