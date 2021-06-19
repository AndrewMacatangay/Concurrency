#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <future>
using namespace std;

class Tree
{
	private:

	struct Node
	{
		Node* left;
		Node* right;
		int value;

		Node(int v) : left(nullptr), right(nullptr), value(v) { }
	};

	Node* head;
	int threadLimit;
	int numNodes;

	public:

	Tree(int t, int n) : head(nullptr), threadLimit(t), numNodes(n)
	{
		vector<future<void>> futures;
		int threadCount = 0;

		//Limit number of threads to N + 1, where N is the number of cores in CPU
		for (int x = 0; x < numNodes; x++)
		{
			//addNode(nullptr, head, rand() % numNodes + 1);
			//async can start a new thread, or optimize and use current thread
			if (threadCount < threadLimit)
			{
				futures.push_back(async(&Tree::addNode, this, nullptr, ref(head), rand() % numNodes + 1));
				threadCount++;
			}
			else
			{
				futures.clear();
				threadCount = 0;
			}
		}
	}

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
			this_thread::sleep_for(1ms);
			return;
		}
		else if (cur->value == value)
			return;

		addNode(cur, (cur->value > value ? cur->left : cur->right), value);
	}

	void _inOrder(Node* cur)
	{
		if (!cur)
			return;

		_inOrder(cur->left);
		cout << cur->value << " ";
		_inOrder(cur->right);
	}

	void inOrder()
	{
		_inOrder(head);
	}
};

int main(int argc, char** argv)
{
	Tree tree(stoi(argv[1]), stoi(argv[2]));

	if (argc > 3 && stoi(argv[3]))
	{
		tree.inOrder();
		cout << endl;
	}

	return 0;
}
