#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<future>
using namespace std;

class Tree
{
	private:

	struct Node
	{
		int value;
		Node* left;
		Node* right;

		Node(int v) : value(v), left(nullptr), right(nullptr) { }
	};

	Node* head;
	
	public:

	Tree(int numNodes)
	{
		//srand(time(NULL));
		head = nullptr;

		for(int x = 0; x < numNodes; x++)
			insertInOrder(nullptr, head, rand() % numNodes + 1);
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

	void insertInOrder(Node* parent, Node*& cur, int value)
	{
		if (!parent && !cur)
		{
			cur = new Node(value);
			return;
		}
		else if (!cur)
		{
			(parent->value < value ? parent->right : parent->left) = new Node(value);
			return;
		}
		else if (cur->value == value)
			return;

		cur->value > value ? insertInOrder(cur, cur->left, value) : insertInOrder(cur, cur->right, value);
	}

	int _search(Node* cur, int value)
	{
		if (!cur)
			return -1;
		if (cur->value == value)
			return value;
	
		future<int> leftFuture = async(&Tree::_search, this, cur->left, value);
		future<int> rightFuture = async(&Tree::_search, this, cur->right, value);

		int leftFound = leftFuture.get();
		int rightFound = rightFuture.get();

		if (leftFound == rightFound)
			return -1;

		return leftFound > rightFound ? leftFound : rightFound;

		//cur->value > value ? search(cur->left, value) : search(cur->right, value);
	}

	int search(int value)
	{
		return _search(head, value);
	}
};

int main()
{
	Tree tree(1000);
//	tree.inOrder();

	cout << endl;
	cout << "Search result: " << tree.search(588) << endl;

	return 0;
}
