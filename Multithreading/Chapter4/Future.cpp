//This program demonstrates the use of a future. Let's
//say that I want a parent thread to be able to access some
//data that a child thread has produced. From what we know
//so far, we would have to pass a reference to the function
//and then wait for that thread to produce the object. We end
//up needing a mutex to protect the shared data, and a
//condition_variable to wait for the thread to finish running.
//Instead, we can use a future to do that work for us!

#include <iostream>
#include <string>
#include <thread>
#include <future>
using namespace std;

string getMessage()
{
	return "This is Andrew 20 years from the future!";
}

int main()
{
	cout << "This is Andrew!" << endl;

	future<string> f = async(getMessage);
	cout << f.get() << endl;

	cout << "This is Andrew 40 years from the future!" << endl;
	
	return 0;
}
