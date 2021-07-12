/*
Free functions for the atomic class are designed to be C-compatible. They take
the address of an atomic rather than a reference. If you're working in C++,
this may not need to be used at all.
*/

#include <iostream>
#include <atomic>
using namespace std;

int main()
{
	atomic<int> a(1);
	
	cout << "atomic_load(): " << atomic_load(&a) << endl;
	cout << "load(): " << a.load() << endl;

	return 0;
}
