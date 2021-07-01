//This program is a concurrent tree class that supports the concurrent adding and searching
//of nodes. The addNode() function uses hand-over-hand locking to add a node to the list. Each
//thread will be assigned one node to add to the tree. The search() function will utilize
//futures to traverse down both the left and right subtrees of each node to try and find the
//value we are looking for. If the tree is a binary search tree, the overhead for creating
//futures is unnecessary. If the tree is not sorted, this may be useful. Although the
//functions are thread-safe, they perform worse than a single-threaded implementation.
//Blocking algorithms are being used for both functions which will affect run-time. This
//program is a good proof-of-concept that can be used for structures such as graphs.

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
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
	mutex m;
	once_flag initFlag;
	atomic<int> processed;

	public:
	
	//When a tree is constructed, a new thread will be launched to add a ndoe to the tree.
	//This can create bottlenecks since the number of threads concurrently running is
	//not bounded. I can fix this in the future.
	Tree(int numNodes) : root(nullptr), processed(0)
	{
		//Uncomment this to have truly random values. Comment it for testing purposes
		//srand(time(NULL));
		for (int x = 0; x < numNodes; x++)
			thread(&Tree:: addNode, this, nullptr, ref(root), rand() % numNodes + 1).detach();
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

			//Creating a new node above will have an unlocked mutex
			//and unlocking the parent will end the hand-over-hand locking
			parent->m.unlock();

			//If we are storing the threads in containers, we might want
			//to let the thread sleep to "offset" the costs of creating
			//and utilizing the vector
			//this_thread::sleep_for(1ms);
			return;
		}

		//If we run into a duplicate number, then unlock the current
		//lock to end the hand-over-hand locking
		else if (cur->value == value)
		{
			cur->m.unlock();
			if (!parent)
				processed++;
			return;
		}
		
		//Determine which direction we want to visit next
		bool lr = cur->value > value;

		//If the direction we want to visit next is occupied,
		//then lock that noce and unlock the current node
		//(hand-over-hand locking)
		if ((lr && cur->left) || (!lr && cur->right))
		{
			(lr ? cur->left : cur->right)->m.lock();
			cur->m.unlock();
		}

		//Visit the next node
		addNode(cur, lr ? cur->left : cur->right, value);
		if (!parent)
			processed++;
	}

	//Prints out the tree in-order of value (ascending)
	void _inOrder(Node* cur)
	{
		if (!cur)
			return;

		_inOrder(cur->left);
		cout << cur->value << " ";
		_inOrder(cur->right);
	}
	
	//Calls a recursive helper function to print out
	//each value in order
	void inOrder()
	{
		_inOrder(root);
	}

	//Searches the tree to find a given value. This function is designed for a tree
	//that is unsorted. A sorted tree would perform faster since we know the exact
	//path to take. In an unsorted tree, will brute-force and concurrently visit all nodes
	int _search(Node* cur, int value)
	{
		//If we find a don't find the node, return -1, otherwise return the value
		//if found
		if (!cur)
			return -1;
		if (cur->value == value)
			return value;

		//Create two futures that will recursively call _search() and will hold
		//the value of -1 or "value" if it is found in the sub-tree
		future<int> leftFuture = async(&Tree::_search, this, cur->left, value);
		future<int> rightFuture = async(&Tree::_search, this, cur->right, value);

		//Block until the value from the future is ready
		int leftFound = leftFuture.get();
		int rightFound = rightFuture.get();

		//If the values are unique, then -1 is the only option to have duplicate
		//return values for the sub-trees. This means we didn't find the node
		if (leftFound == rightFound)
			return -1;

		//If leftFound and rightFound are different, then the higher number
		//is the value we are looking for, and it should be returned
		return leftFound > rightFound ? leftFound : rightFound;

	}

	//Calls a the helper function _search() to look for a specific node in the tree
	int search(int value)
	{
		return _search(root, value);
	}

	//Returns the number of values that have been processed for the tree
	int getProcessed()
	{
		return processed;
	}
};

int main(int argc, char** argv)
{
	//Number of threads the number of nodes
	int size = stoi(argv[1]);
	Tree tree(size);

	//Wait for all threads to get processed before proceeding
	while(tree.getProcessed() != size);

	if (argc > 2 && stoi(argv[2]))
	{
		tree.inOrder();
		cout << endl;
		//cout << "Searching for 588: " << (tree.search(588) > 0 ? "Found" : "Not Found") << endl;
	}

	return 0;
}
