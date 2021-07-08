#include <iostream>
#include <type_traits>
#include <atomic>
using namespace std;

struct Node
{
	int value;
	Node* next;
};

int main()
{
	cout << "Node: " << is_trivially_copyable<Node>::value << endl;
	cout << "Node to Node: " << is_trivially_copy_assignable<Node>::value << endl;
	atomic<Node> n;
	n.store({1, nullptr});

	cout << n.load().value << endl;

	return 0;
}
