#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <future>
using namespace std;

//int threadCount = 0;

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
		//this_thread::sleep_for(1ms);
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

int main(int argc, char** argv)
{
	Node* head = nullptr;
	//vector<future<void>> futures;
	//int threadLimit = atoi(argv[1]);
	
	//Limit number of threads to N + 1, where N is the number of cores in CPU
	for (int x = 0; x < 100000; x++)
	{
		addNode(nullptr, head, rand() % 100000 + 1);
		//async can start a new thread, or optimize and use current thread
		/*if (threadCount < threadLimit)
		{
			futures.push_back(async(addNode, nullptr, ref(head), rand() % 100000 + 1));
			threadCount++;
		}
		else
		{
			futures.clear();
			threadCount = 0;
		}*/
	}

	//for(future<void>& f : futures)
		//f.get();

	//inOrder(head);
	//cout << endl;

	return 0;
}
