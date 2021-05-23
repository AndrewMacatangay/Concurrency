//This program demostrates the use of threads
//contained within a vector and passing
//parameters into the thread of execution.
//Note that race conditions occur within
//this code!

#include<iostream>
#include<vector>
#include<thread>
using namespace std;

void func(int x)
{
	cout << "This is thread number: " << x << endl;
}

int main()
{
	vector<thread> threads;

	for (int x = 1; x <= 5; x++)
		threads.push_back(thread(func, x));

	for (thread& t : threads)
		t.join();

	return 0;
}
