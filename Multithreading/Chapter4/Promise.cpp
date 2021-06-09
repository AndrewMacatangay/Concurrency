#include <iostream>
#include <functional>
#include <thread>
#include <future>
using namespace std;

class BankAccount
{
	private:
		double balance;
	public:
		BankAccount(double balance) : balance(balance){ }

		double invest(future<double>& promisedLoan)
		{
			double loan = promisedLoan.get();
			balance += loan;

			//Do some investing here to make money

			double interest;
			return loan + interest; 
		}
};

class Contract
{
	private:
		BankAccount& Lender, Lendee;
		double loan, interest;
	public:
		Contract(BankAccount& Lender, BankAccount& Lendee) : Lender(Lender), Lendee(Lendee){ }

		void invokeContract(double loan, double interest)
		{

			this->loan = loan;
			this->interest = interest;

			promise<double> p;

			future<double> f1 = p.get_future();
			
			future<double> f2 = async(&BankAccount::invest, &Lendee, ref(f1));

			p.set_value(100);

			cout << f2.get() << endl;
		}
};

int main()
{
	BankAccount Lender(1000000);
	BankAccount Lendee(100000);

	Contract loan(Lender, Lendee);
	loan.invokeContract(1000, 1000);

	return 0;
}
