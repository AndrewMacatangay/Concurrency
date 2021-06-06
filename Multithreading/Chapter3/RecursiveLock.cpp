//This program is a demonstration of a recursive
//lock. Usage for recursive locks are not recommended
//since many pitfalls can arise. There may be good
//uses for recursive functions. If this technique
//seems to be appropriate, you should consider if
//the code design is appropriate first.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

recursive_mutex m;

void recursivePrint(int count)
{
	if (!count)
		return;

	cout << "Count: " << count << endl;
	lock_guard<recursive_mutex> l(m);
	recursivePrint(count - 1);
}

int main()
{
	recursivePrint(5);

	return 0;
}
