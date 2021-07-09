#include <iostream>
#include <type_traits>
#include <atomic>
using namespace std;

struct Node
{
	int value;
	atomic<Node*> next;
	Node(int v = -1, Node* n = nullptr) : value(v), next(n) { }
};

atomic<Node*> head(nullptr);
atomic<Node*> tail(nullptr);

void push_back(int value)
{
	if (!head)
	{
		head = tail = new Node(value);
		return;
	}
	atomic<Node*> newNode(new Node(value));	
	tail.load()->next = newNode.load();
	tail = newNode.load();

}

void print()
{
	for(Node* cur = head; cur; cur = cur->next)
		cout << cur->value << " ";
	cout << endl;
}

int main()
{
	cout << "Node: " << is_trivially_copyable<Node>::value << endl;
	cout << "Node to Node: " << is_trivially_copy_assignable<Node>::value << endl;

	for(int x = 0; x < 5; x++)
		push_back(x);

	print();

	return 0;
}
