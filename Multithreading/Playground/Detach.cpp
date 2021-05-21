#include<iostream>
#include<string>
#include<limits>
#include<thread>
using namespace std;

static int stall = 0;

void func1()
{
	int counter = 1;
	while(1)
	{
		cout << "\033c";
		cout << "Type and enter '1' to stop the program!" << endl;
		cout << "Wrong inputs will result in a 5 second stall." << endl;
		cout << "Thread 1 ran for " << counter++ << " second(s)... " << endl;
		cout << "Current stall: " << (stall ? stall-- : stall) << " seconds" << endl;
		this_thread::sleep_for(1s);
	}
}


//Todo: Stop cin from getting user input while a stall exists
void func2()
{
	string input;
	//bool second = false;
	
	for (cin >> input; 1;)
	{
		if (input != "1")
		{
			cout << "Wrong password, stalled for 5 seconds" << endl;
			stall += 5;
			this_thread::sleep_for(5s);
		}
		else
			break;
		
		if (!stall)
			cin >> input;
	}
}

int main()
{	
	//Thread 1 prompts the user for the input and runs until thread 2 stops it.
	thread t1(func1);
	t1.detach();
	
	//Thread 2 takes in the input and the program ends when the correct input is given.
	thread t2(func2);
	t2.join();
	
	return 0;
}