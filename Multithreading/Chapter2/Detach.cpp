#include<iostream>
#include<string>
#include<thread>
#include<chrono>
using namespace std;

static int stall = 0;

//Displays the current information
void func1()
{
	int seconds = 1;
	
	while(1)
	{
		cout << "\033c";
		cout << "Type and enter '1' to stop the program!" << endl;
		cout << "Wrong inputs will result in a 5 second stall." << endl;
		cout << "Thread 1 ran for " << seconds++ << " second(s)... " << endl;
		cout << "Current stall: " << (stall ? stall-- : 0) << " seconds" << endl;
		this_thread::sleep_for(1s);
	}
}

//Prompts the user for input
void func2()
{
	string input;
	
	//Continuously ask for input until the correct input is given
	while (cin >> input)
	{
		//Add a penalty of 5 seconds if the input is wrong
		if (input != "1")
		{
			cout << "Wrong input! Stalled for 5 seconds" << endl;
			stall += 5;
		}
		//If the input is correct, stall the thread for the duration of the stall
		else
		{
			this_thread::sleep_for(chrono::seconds(stall));
			break;
		}
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