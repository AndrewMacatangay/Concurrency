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
			int localSum = 0;

			for(int x = 1; x < 6; x++)
			{
				//unique_lock<mutex> l(m);
				//l.lock();
				localSum += x;
				//l.unlock();
			}
			
			unique_lock<mutex> l(m);
			l.lock();
			sum = localSum;
			l.unlock();
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
	//this_thread::sleep_for(3s);
	cout << mySum.getSum() << endl;
	return 0;
}
