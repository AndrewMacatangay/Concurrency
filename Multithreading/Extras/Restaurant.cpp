#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Restaurant
{
	private:
		thread info, burgerStation, randomOrder;
		mutex burgerMutex;
		int seconds, burgers, burgersSold, missedOrders;

		void print_info()
		{
			while(1)
			{
				cout << "\033c";
				cout << "A burger is made every 3 seconds." << endl;
				cout << "Time: " << seconds++ << " second(s)" << endl;
				
				if (burgers)
					cout << "Burgers: " << burgers << endl;
				
				cout << "Burgers sold: " << burgersSold << endl;
				cout << "Missed orders: " << missedOrders << endl;
				this_thread::sleep_for(1s);
			}
		}
		
		void make_order()
		{
			while(1)
			{
				this_thread::sleep_for(3s);
				burgers ? burgers--, burgersSold++ : missedOrders++;		
			}
		}
		void make_burger()
		{
			while(1)
			{
				this_thread::sleep_for(3s);
				burgers++;	
			}
		}

	public:
		Restaurant()
		{
			seconds = burgers = burgersSold = missedOrders = 0;
			info = thread(&Restaurant::print_info, this);
			randomOrder = thread(&Restaurant::make_order, this);
			burgerStation = thread(&Restaurant::make_burger, this);	
		}

		~Restaurant()
		{
			info.join();
			burgerStation.join();
		}
};

int main()
{
	Restaurant R;
	
	return 0;
}
