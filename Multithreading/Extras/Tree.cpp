#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
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
	int numNodes;
	mutex m;
	once_flag initFlag;

	public:
	
	//When a tree is constructed, a new thread will be launched to add a ndoe to the tree.
	//This can create bottlenecks since the number of threads concurrently running is
	//not bounded. I can fix this in the future.
	Tree(int n) : root(nullptr), numNodes(n)
	{
		for (int x = 0; x < numNodes; x++)
			thread(&Tree::addNode, this, nullptr, ref(root), rand() % numNodes + 1).detach();
	}

	//Clear every node in the tree
	void clear(Node* cur)
	{
		if (!cur)
			return;

		clear(cur->left);
		clear(cur->right);
		delete cur;
	}

	//Destructor will call the clear function
	~Tree()
	{
		clear(root);
	}

	//Thread-safe function to add a node to the tree.
	//Hand-over-hand locking is used to avoid race conditions.
	void addNode(Node* parent, Node*& cur, int value)
	{
		//We want to block here to initialize the root node.
		//This only needs to be done once, so we used call_once
		call_once(initFlag, [&]{ cur = new Node(value); });
		
		//If we are at the root node, then we want to lock it so that
		//no other node can occupy this space (hand-over-hand).
		if (!parent && root)
			root->m.lock();

		//If we are at nullptr, then we want to add the new node here
		if (!cur)
		{
			//Find which direction from the parent we should add the
			//new node in and add it
			bool lr = parent->value > value;
			(lr ? parent->left : parent->right) = new Node(value);

			//Creating a new node above will lock that new node and 
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

		_inOrder(cur->left);
		_inOrder(cur->right);
		cout << cur->value << " ";
	}

	void inOrder()
	{
		_inOrder(root);
	}
};

int main(int argc, char** argv)
{
	//Number of threads the number of nodes
	Tree tree(stoi(argv[1]));

	if (argc > 2 && stoi(argv[2]))
	{
		tree.inOrder();
		cout << endl;
	}

	return 0;
}
