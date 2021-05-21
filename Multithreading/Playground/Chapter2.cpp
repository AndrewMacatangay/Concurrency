#include <iostream>
#include <thread>
using namespace std;

void task1()
{
	for (int x = 1; x < 1000001; x++)
		int y = x * 5;
		//cout << "Worker 1 is on task " << x << "!" << endl;
}

void task2()
{
	for (int x = 1; x < 1000001; x++)
		int y = x * 5;
		//cout << "Worker 2 is on task " << x << "!" << endl;
}

int main()
{
		for (int x = 1; x < 1000001; x++)
			int y = x * 5;
		for (int x = 1; x < 1000001; x++)
			int y = x * 5;
	/*thread worker1(task1);
	thread worker2(task2);

	worker1.join();
	worker2.join();*/
	
	return 0;
}
