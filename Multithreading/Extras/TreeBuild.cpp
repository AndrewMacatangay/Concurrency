#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <future>
using namespace std;

class Node
{
		public:
		Node* left;
		Node* right;
		int value;

		Node(int v) : left(nullptr), right(nullptr), value(v) { }
};

void addNode(Node* parent, Node*& cur, int value)
{
	if(!parent && !cur)
	{
		cur = new Node(value);
		return;
	}
	else if (!cur)
	{	
		(parent->value > value ? parent->left : parent->right) = new Node(value);
		return;
	}
	else if (cur->value == value)
		return;

	addNode(cur, (cur->value > value ? cur->left : cur->right), value);
}

void inOrder(Node* cur)
{
	if (!cur)
		return;

	inOrder(cur->left);
	cout << cur->value << " ";
	inOrder(cur->right);
}

int main()
{
	Node* head = nullptr;
	vector<future<void>> futures;

	for (int x = 0; x < 10000; x++)
	{
		//addNode(nullptr, head, rand() % 10000 + 1);
		futures.push_back(async(addNode, nullptr, ref(head), rand() % 10000 + 1));
	}

	for(future<void>& f : futures)
		f.get();

	//inOrder(head);
	//cout << endl;

	return 0;
}
