//Just a simple example of running 2 worker threads

#include <iostream>
#include <thread>
using namespace std;

void task1()
{
	for (int x = 1; x <= 5; x++)
		cout << "Worker 1 is on task " << x << "!" << endl;
}

void task2()
{
	for (int x = 1; x <= 5; x++)
		cout << "Worker 2 is on task " << x << "!" << endl;
}

int main()
{
	thread worker1(task1);
	thread worker2(task2);

	worker1.join();
	worker2.join();
	
	return 0;
}
