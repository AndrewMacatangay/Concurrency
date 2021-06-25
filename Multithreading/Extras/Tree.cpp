#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <future>
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

	Node* head;
	int threadLimit;
	int numNodes;
	mutex m;
	once_flag initFlag;

	public:

	Tree(int t, int n) : head(nullptr), threadLimit(t), numNodes(n)
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
		clear(head);
	}

	void addNode(Node* parent, Node*& cur, int value)
	{
		call_once(initFlag, [&]{ cur = new Node(value); });
		
		if (!parent && head)
			head->m.lock();

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
		if (lr && cur->left)
		{
			cur->left->m.lock();
			cur->m.unlock();
		}
		else if (!lr && cur->right)
		{
			cur->right->m.lock();
			cur->m.unlock();
		}

		addNode(cur, lr ? cur->left : cur->right, value);
		
		/*if (!parent && head)
			head->m.lock();

		if (value < cur->value)
		{
			if (!cur->left)
			{
				cur->left = new Node(value);
				cur->m.unlock();
			}
			else
			{
				cur->left->m.lock();
				cur->m.unlock();
				addNode(cur, cur->left, value);
			}
		}
		else if (value > cur->value)
		{
			if (!cur->right)
			{
				cur->right = new Node(value);
				cur->m.unlock();
			}
			else
			{
				cur->right->m.lock();
				cur->m.unlock();
				addNode(cur, cur->right, value);
			}
		}
		else
		{
			cur->m.unlock();
		}*/
	}

	//1/500 chance of missing one node
	void _inOrder(Node* cur)
	{
		if (!cur)
			return;

		cout << cur->value << " ";
		_inOrder(cur->left);
		//cout << cur->value << " ";
		_inOrder(cur->right);
	}

	void inOrder()
	{
		_inOrder(head);
		//cout << endl << count << endl;
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
