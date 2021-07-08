/*
This program is intended to demonstrate pointer arithmetic and atomic operations.
Note that fetch instructions will return the original value after performing
the operation, and the compound assignment, increment, and decrement operators
will return the result after performing the operation.
*/

#include <iostream>
#include <atomic>
using namespace std;

int main()
{
	//Create an array of 5 elements
	int myArr[5];

	//Create an atomic pointer that points to the beginning of "myArr"
	atomic<int*> atomicPointer(myArr);

	//Add 2 to the pointer. Fetch will return the beginning of the array
	int* pointer = atomicPointer.fetch_add(2);

	if (myArr == pointer)
		cout << "fetch_add() returned the old value!" << endl;
	if (atomicPointer.load() == &myArr[2])
		cout << "fetch_add() successfully updated the pointer!" << endl;

	//Subtract 1 from the pointer. Fetch will return index 1 of the array
	pointer = atomicPointer -= 1;

	if (pointer == &myArr[1])
		cout << "-= returned the new value!" << endl;
	if (atomicPointer.load() == &myArr[1])
		cout << "-= successfully updated the pointer!" << endl;

	return 0;
}
