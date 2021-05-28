#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

//Protect from passing functions/references/pointers

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

		//This is like bringing documents from a company
		//home and then them getting leaked.
		int& getCompanyBalanceRef()
		{
			lock_guard<mutex> l(m);
			return companyBalance;
		}

		//
		int* getCompanyBalanceVal()
		{
			lock_guard<mutex> l(m);
			return &companyBalance;
		}
};

int main()
{
	Company Amazon;
	
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	int& newBalance = Amazon.getCompanyBalanceRef();
	newBalance = 1000000;
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	*(Amazon.getCompanyBalanceVal()) = 1000;
	cout << "Company balance: " << Amazon.getCompanyBalance() << endl;
	
	return 0;
}
