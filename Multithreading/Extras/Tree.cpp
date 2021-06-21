#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <future>
#include <atomic>
#include <mutex>
#include <condition_variable>
using namespace std;

class Tree
{
	private:

	struct Node
	{
		Node* left;
		Node* right;
		int value;
		atomic<int> leftNodes;
		atomic<int> rightNodes;
		
		Node(int v) : left(nullptr), right(nullptr), value(v) , leftNodes(0), rightNodes(0) { }
	};

	Node* head;
	//Make these atomic as well
	int threadLimit;
	int numNodes;
	mutex m;
	condition_variable cv;
	atomic<int> count;

	public:

	Tree(int t, int n) : head(nullptr), threadLimit(t), numNodes(n), count(0)
	{
		vector<future<void>> futures;
		atomic<int> threadCount(0);

		//Limit number of threads to N + 1, where N is the number of cores in CPU
		for (int x = 0; x < numNodes; x++)
		{
			//addNode(nullptr, head, rand() % numNodes + 1);
			//async can start a new thread, or optimize and use current thread
			futures.push_back(async(launch::async, &Tree::addNode, this, nullptr, ref(head), rand() % numNodes + 1));
			threadCount++;
			
			if (threadCount > threadLimit)
			{
				futures.clear();
				threadCount = 0;
			}
		}
	}

	void addNode(Node* parent, Node*& cur, int value)
	{
		//Find a way to optimize this lock.
		//The problem starts with 2 threads trying to establish a root node.
		//unique_lock<recursive_mutex> l(m);
		//if(!parent && !cur)
		if (!count)
		{
			count++;
			cur = new Node(value);
			return;
		}
		else if(!cur)
		{
			bool lr = parent->rightNodes > parent->leftNodes;

			unique_lock<mutex> l(m);
			//Retry node
			if ((lr && parent->left) || (!lr && parent->right))
			{	
				l.unlock();
				//Current is null, you are passing nullptr
				//again when the parent's left/right has been updated
				addNode(parent, cur, value);
			}
			count++;
			(lr ? parent->left : parent->right) = new Node(value);
			lr ? parent->leftNodes++ : parent->rightNodes++;
			l.unlock();
			this_thread::sleep_for(1ms);
			return;
		}
		//else if (cur->value == value)
		//	return;
		addNode(cur, (cur->rightNodes > cur->leftNodes ? cur->left : cur->right), value);
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
