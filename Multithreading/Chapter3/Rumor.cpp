#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
//Protecting shared data
//Protect from passing functions/references/pointers

//This is a malicious function that can be passed into
//one of the Company member functions to zero out the
//balance
void badFunc(int& balance)
{
	balance = 0;
}

class Company
{
	private:
		int companyBalance;
		mutex m;
	public:
		Company()
		{
			companyBalance = 1000000000;
		}

		int getCompanyBalance()
		{
			return companyBalance;
		}

		int& getCompanyBalanceRef()
		{
			lock_guard<mutex> l(m);
			return companyBalance;
		}

		int* getCompanyBalanceVal()
		{
			lock_guard<mutex> l(m);
			return &companyBalance;
		}

		void runFunc(void fun(int&))
		{
			fun(companyBalance);
		}
};

//An example of what not to do
int main()
{
	//The company is initialized with a billion dollars
	Company Amazon;	
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	
	//After passing a reference back to main, we can access the balance
	//without going through the mutex
	int& newBalance = Amazon.getCompanyBalanceRef();
	newBalance = 1000000;
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	
	//After passing the address back to main, we can access the balance
	//without going through the mutex
	*(Amazon.getCompanyBalanceVal()) = 1000;
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	
	//After passing a function for the member function to execute, we
	//can access the balance without going through the mutex
	Amazon.runFunc(badFunc);
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	
	return 0;
}
