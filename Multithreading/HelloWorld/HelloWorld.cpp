#include <iostream>
#include <thread>
using namespace std;

void hello()
{
	cout << "Hello World!" << endl;
}

int main()
{
	std::thread t(hello);
	t.join();
	
	return 0;
}