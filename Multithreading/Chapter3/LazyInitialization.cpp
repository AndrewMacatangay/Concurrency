#include <iostream>
#include <vector>
#include <mutex>
using namespace std;

class Example
{
	private:
		vector<int> v;
		mutex m;
		once_flag initFlag;
	public:
		Example()
		{
			//Instead of initializing here, we
			//will only initialize a variable
			//when needed.

			//v = vector<int>(100, 0);
		}

		int readSomething(int index)
		{	
			if (index < 0 || index > 99)
				return -1;

			//Although this code works, a new
			//unique_lock will be constructed
			//at each function call, even if
			//the vector has been initialized
			
			//unique_lock<mutex> l(m);
			//if (v.empty())
			//v = vector<int>(100, 0);
			//l.unlock();

			//Using call_once is a better alternative
			call_once(initFlag, [&](){ v = vector<int>(100, 0);});
			return v[index];
		}

		void writeSomething(int index, int value)
		{
			if (index < 0 || index > 99)
				return;

			call_once(initFlag, [&](){ v = vector<int>(100, 0);});
			v[index] = value;
		}
};

int main()
{
	Example e;
	int index = 1;

	e.writeSomething(index, 50);
	cout << "Read in the value " << e.readSomething(index) << " from index " << index << endl;
	return 0;
}
