//This example of fine grained locks performs better
//since it allows for greater concurrency. Instead of 
//acquiring locks within each iteration of the loop,
//a the sum is stored into a local variable rather than
//needing to repeatedly access the shared resource "sum".
//The local sum's value will then be assigned to the 
//shared sum variable which will only require one access
//to the shared resource per thread.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

long sum = 0;
mutex m;

void func1()
{
	long localSum = 0;
	
	for(int x = 1; x <= 500000; x++)
		localSum += x;
	
	unique_lock<mutex> l(m);
	sum += localSum;
}

void func2()
{
	long localSum = 0;
	
	for(int x = 500001; x <= 1000000; x++)
		localSum += x;

	unique_lock<mutex> l(m);
	sum += localSum;
}

int main()
{
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();
	cout << "Sum: " << sum << endl;
}