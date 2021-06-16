#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<thread>
#include<mutex>
using namespace std;

struct Node
{
	int value;
	Node* left;
	Node* right;

	Node(int v) : value(v), left(nullptr), right(nullptr) { }
};

void inOrder(Node* cur)
{
	if (!cur)
		return;
	
	inOrder(cur->left);
	cout << cur->value << " ";
	inOrder(cur->right);
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

int search(Node* cur, int value)
{
	if (!cur)
		return -1;
	if (cur->value == value)
		return value;
	
	//Add a future here?

	cur->value > value ? search(cur->left, value) : search(cur->right, value);
}

int main()
{
	//srand(time(NULL));

	Node* head = nullptr;
	
	for(int x = 0; x < 1000; x++)
	{
		int random = rand() % 1000 + 1;
		//cout << random << " ";
		insertInOrder(nullptr, head, random);
	}

	//cout << endl;
	inOrder(head);

	cout << "Search result: " << search(head, 588) << endl;

	return 0;
}
