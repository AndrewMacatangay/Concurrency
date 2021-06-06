//This program demonstrates the use of conditon variables to
//blocks thread from executing while waiting for another thread.
//The program will add a value to the vector if the vector is empty
//and will remove a value from the vector if the vector is not
//empty.

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex m;
condition_variable cv;
vector<int> v;
int count = 0;

void add()
{
	//Add an element to the vector and notify the
	//waiting thread when finished
	while(count != 10)
	{
		unique_lock<mutex> l(m);
		cv.wait(l, []{ return v.empty(); });
		v.push_back(count);
		cout << "Added " << count++ << " to the vector!" << endl;
		cv.notify_one();
	}
}

void subtract()
{
	//Remove an element from the vector and notify the
	//waiting thread when finished
	while(count != 10)
	{
		unique_lock<mutex> l(m);
		cv.wait(l, []{ return !v.empty(); });
		int value = v.back();
		v.pop_back();
		cout << "Value: " << value << " Size: " << v.size() << endl;
		cv.notify_one();
	}
}

int main()
{
	thread t1(add);
	thread t2(subtract);

	t1.join();
	t2.join();

	return 0;
}
