//This program demonstrates an example of a deadlock that
//occurs when locks are in the proper order. A for loop has
//been added in between the two locks so that the deadlock
//will actually get caught. Without it, the theads perform
//fast enough to avoid the deadlock.
//The first thread will lock mutex 1, the second thread will
//lock mutex 2, then as thread 1 attempts to lock mutex 2,
//thread 2 has already locked it. Thread 2 cannot lock mutex 1
//since thread 1 has already locked it. Deadlock!

#include<iostream>
#include<algorithm>
#include<thread>
#include<mutex>
using namespace std;

class someClass
{
	private:
		int threadNumber, data;
		mutex m;
	public:
		someClass(int d)
		{
			threadNumber = data = d;
		}

		//This is a deadlock situation.
		//Lets put some code in between the two locks to
		//actually catch the deadlock
		void exchangeData(someClass& other)
		{	
			lock_guard<mutex> l1(m);
			for(int x = 0; x < 20000; x++);
			lock_guard<mutex> l2(other.m);			
			swap(data, other.data);
			cout << "Thread number: " << threadNumber << endl;
			cout << "Data: " << data << endl << endl;
		}
};

int main()
{
	someClass class1(1), class2(2);

	//Important to note that if I wanted the thread to call
	//class1.exchangeData(class2), I need to pass the function
	//with the proper namespace and references to the class objects
	thread t1(&someClass::exchangeData, ref(class1), ref(class2));
	thread t2(&someClass::exchangeData, ref(class2), ref(class1));

	t1.join();
	t2.join();
}
