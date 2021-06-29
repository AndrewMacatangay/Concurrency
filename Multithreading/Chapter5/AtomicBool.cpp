#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

atomic_flag flag = ATOMIC_FLAG_INIT;
int count = 0;

void print(int threadNumber)
{	
	while(flag.test_and_set());
	
	cout << "Thread Number: " << threadNumber << endl;
	count++;
	
	flag.clear();
}

int main()
{
	for (int x = 0; x < 10; x++)
		thread(print, x).detach();

	while(count ^ 10);

	return 0;
}
