#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Sum
{
	private:
		int sum;
		thread t1, t2;
		mutex m;

		void Sum1to5()
		{
			for(int x = 1; x < 6; x++)
				sum += x;
		}
	public:
		Sum()
		{
			sum = 0;
			t1 = thread(&Sum::Sum1to5, this);
			t2 = thread(&Sum::Sum1to5, this);
		}
		
		~Sum()
		{
			t1.join();
			t2.join();
		}

		int getSum()
		{
			return sum;
		}
};

int main()
{
	Sum mySum;
	this_thread::sleep_for(3s);
	cout << mySum.getSum() << endl;
	return 0;
}
