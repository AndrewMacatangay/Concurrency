#include<iostream>
#include<thread>
#include<future>
using namespace std;

int task(int x)
{
	cout << "The task has been run!" << endl;
	return x;
}

//You want it so that certain threads will run certain
//tasks. Move the tasks into a thread.

int main()
{
	packaged_task<int(int)> pt(task);
	future<int> f = pt.get_future();
	pt(5);
	
	cout << f.get() << endl;
	return 0;
}
