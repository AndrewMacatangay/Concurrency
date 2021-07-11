/*
Atomic user-defined types are more likely than not, trivially copy-assignable.
This allows the compiler to use memcpy() on an object. This is necessary for the
object to be compare-exchanged. Keep in mind that an atomic struct is different 
than a struct with atomics inside of it.
*/

#include <iostream>
#include <type_traits>
#include <atomic>
using namespace std;

struct Node
{
	int value;
	Node(int v = -1) : value(v) { }
};

int main()
{
	cout << "Trivially copy assignable: " << is_trivially_copy_assignable<Node>::value << endl;

	atomic<Node> n(4);
	cout << "Node value: " << n.load().value << endl;

	cout << (n.is_lock_free() ? "Lock-Free!" : "Not Lock-Free...") << endl;
	return 0;
}
