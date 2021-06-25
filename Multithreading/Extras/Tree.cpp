#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
using namespace std;

class Tree
{
	private:

	struct Node
	{
		Node* left;
		Node* right;
		int value;
		mutex m;		
		Node(int v) : left(nullptr), right(nullptr), value(v) { }
	};

	Node* root;
	int threadLimit;
	int numNodes;
	mutex m;
	once_flag initFlag;

	public:

	Tree(int t, int n) : root(nullptr), threadLimit(t), numNodes(n)
	{
		//vector<future<void>> futures;
		//int threadCount = 0;

		for (int x = 0; x < numNodes; x++)
		{
			//async can start a new thread, or optimize and use current thread
			//futures.push_back(async(launch::async, &Tree::addNode, this, nullptr, ref(root), rand() % numNodes + 1));
			//threadCount++;
			
			//if (threadCount > threadLimit)
			//{
			//	futures.clear();
			//	threadCount = 0;
			//}
			/*future<void> f =*/ thread(&Tree::addNode, this, nullptr, ref(root), rand() % numNodes + 1).detach();
		}
	}

	void clear(Node* cur)
	{
		if (!cur)
			return;

		clear(cur->left);
		clear(cur->right);
		delete cur;
	}

	~Tree()
	{
		clear(root);
	}

	void addNode(Node* parent, Node*& cur, int value)
	{
		call_once(initFlag, [&]{ cur = new Node(value); });
		
		if (!parent && root)
			root->m.lock();

		if (!cur)
		{
			bool lr = parent->value > value;
			(lr ? parent->left : parent->right) = new Node(value);
			parent->m.unlock();
			//this_thread::sleep_for(1ms);
			return;
		}
		else if (cur->value == value)
		{
			cur->m.unlock();
			return;
		}
		
		bool lr = cur->value > value;

		if ((lr && cur->left) || (!lr && cur->right))
		{
			(lr ? cur->left : cur->right)->m.lock();
			cur->m.unlock();
		}

		addNode(cur, lr ? cur->left : cur->right, value);
	}

	void _inOrder(Node* cur)
	{
		if (!cur)
			return;

		cout << cur->value << " ";
		_inOrder(cur->left);
		_inOrder(cur->right);
	}

	void inOrder()
	{
		_inOrder(root);
	}
};

int main(int argc, char** argv)
{
	//Number of threads the number of nodes
	Tree tree(stoi(argv[1]), stoi(argv[2]));

	if (argc > 3 && stoi(argv[3]))
	{
		tree.inOrder();
		cout << endl;
	}

	return 0;
}
