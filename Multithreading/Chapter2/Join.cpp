//A program to showcase join(). The main thread will verify the input while the
//worker thread will provide information. Once the correct input is given, the
//worker thread will join with the main thread amd the program will terminate.

#include <iostream>
#include <string>
#include <thread>
using namespace std;

static bool keepGoing = true;
static int count = 0;

void waitForInput()
{
	while(keepGoing)
	{
		cout << "\033c";
		cout << "Type the alphabet in 6 seconds!" << endl;
		cout << "Failed " << count++ << " attempts(s)..." << endl;
		cout << "Alphabet: " << flush;
		this_thread::sleep_for(6s);
	}
}

int main()
{
	string input;
	thread t(waitForInput);
	
	for (cin >> input; input != "abcdefghijklmnopqrstuvwxyz"; cin >> input)
		cout << "Failed!" << endl;
	
	keepGoing = !keepGoing;
	
	t.join();
	
	cout << "Success!" << endl;
	return 0;
}
