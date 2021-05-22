//This is just a Hello program using threads

#include <iostream>
#include <thread>
using namespace std;

void hello()
{
	cout << "Hello World!" << endl;
}

int main()
{
	thread t(hello);
	t.join();
	
	return 0;
}
