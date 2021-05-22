#include<iostream>
#include<utility>
#include<thread>
using namespace std;

void func1()
{
	cout << "Running func1" << endl;
}

void func2()
{
	cout << "Running func2" << endl;
}

int main()
{
	//Created t1 to run func1
	thread t1(func1);
	
	//Created t2 with t1's contents transferred to t2
	//t1 is now uninitialized
	thread t2 = move(t1);
	
	//t1 will run func2
	t1 = thread(func2);
	
	//Created t3
	thread t3;
	
	//t2's contents are tranferred to t3
	//t2 is now uninitialized
	t3 = move(t2);
	
	//t1 is initialized and is running func2, so t3
	//cammot be transferred to t3. Comment the line
	//below so that the program can run without any
	//issues
	//t1 = move(t3);
	
	//t1 and t3 are the only variables associated with
	//a thread, so we join them. The only variable that
	//does not have to be joined is t2
	t1.join();
	t3.join();
}
