//Sequentially-consistent ordering orders each operation on a "global" view. This is
//simple to understand as it mimics a single-threaded program. Performance-wise, it is
//the most expensive since it requires all threads to conform to a certain "global"
//order. A diagram will be provided as SequentiallyConsistent.JPG

#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> x, y;
atomic<int> z;

//Store true into x
void writeX()
{
	x = 1;
}

//Store true into y
void writeY()
{
	y = 1;
}

//Wait for x to be true, check y, and if y is true, increment z.
//If y is false, do nothing (this will lead to z = 1 rather than 2)
void readXthenY()
{
	while(!x);
	if (y)
		z++;
}

//Wait for y to be true, check x, and if x is true, increment z.
//If x is false, do nothing (this will lead to z = 1 rather than 2)
void readYthenX()
{
	while(!y);
	if (x)
		z++;
}

int main()
{
	x = y = z = 0;

	thread a(writeX);
	thread b(writeY);
	thread c(readXthenY);
	thread d(readYthenX);

	a.join();
	b.join();
	c.join();
	d.join();

	//Z will always be 1 or 2 since it is guaranteed that x will be
	//stored before y, or y before x. If it is 2, then both stores
	//happen before both reads. Otherwise, if we have interleaving, we
	//may have an output of 1
	cout << "Z: " << z << endl;
}
