//This program demonstates the use of promise and futures. We will use a 
//transaction system to outline how this works. The lendee will lend a lender
//some amount of money. A promise must be made for the lender to recieve
//the money from the lender who will send the money from a separate thread.
//Another future will be made for the lender to recieve the amount due from
//the lendee. Note that this does not protect against race conditions.

#include <iostream>
#include <iomanip>
#include <functional>
#include <thread>
#include <future>
using namespace std;

//The BankAccount class will manage each person's balance
class BankAccount
{
	private:
		double balance;
	public:
		BankAccount(double balance) : balance(balance){ }
		
		//The invest function will be called by the Lendee
		//after recieving the loan
		double invest(future<pair<double, double>>& detailFuture)
		{
			cout << "The lendee is now investing!" << endl;
			
			//The promise is fulfilled when the Lendee 
			//recieves a receipt contract details
			pair<double, double> details = detailFuture.get();
			bankTransaction(details.first);
			cout << "The lendee has $" << balance << endl;

			//Do some investing here to make money
			cout << "The lendee has made $25000!" << endl;
			bankTransaction(25000);
			cout << "The lendee has $" << balance << endl;
			
			//The Lendee will return the loan times an
			//additional interest rate for being allowed
			//to borrow money
			double amountDue = details.first * details.second;
			cout << "The lendee has paid back $" << amountDue << endl;
			bankTransaction(-amountDue);
			return amountDue; 
		}

		//This function will be used for deposits and withdrawals
		void bankTransaction(double amount)
		{
			balance += amount;
		}

		double getBalance()
		{
			return balance;
		}
};

class Contract
{
	private:
		BankAccount &Lender, &Lendee;
		double loan, interest;
	public:
		//Initialization list needed here to initialize the references
		Contract(BankAccount& Lender, BankAccount& Lendee) : Lender(Lender), Lendee(Lendee)
		{
			cout << "The contract is made!" << endl;
		}
		
		//This function will deem the contract active
		void invokeContract(double loan, double interest)
		{
			cout << "The contract has been invoked!" << endl;
			cout << "The loan is $" << loan << " with an interest rate of " << interest << "%" << endl << endl;

			//Contract information is saved internally within the class
			this->loan = loan;
			this->interest = interest;

			//A promise is proposed as a pair of {loan, interest}
			promise<pair<double, double>> p;

			//A future will be made which is associated to the promise
			//which will allow the Lendee to recieve a "receipt"
			future<pair<double, double>> f1 = p.get_future();

			Lender.bankTransaction(-loan);
			cout << "The lender has $" << Lender.getBalance() << endl;

			//A future will be made whichc will allow the Lender to 
			//recieve their loan amount back with interest on top
			future<double> f2 = async(&BankAccount::invest, &Lendee, ref(f1));

			//Send the contract information to the Lendee to verify
			p.set_value({this->loan, this->interest});

			//Complete the transaction by recieving the owed amount
			Lender.bankTransaction(f2.get());
		}
};

int main()
{
	//Create two BankAccounts that will perform a lending transaction
	BankAccount Lender(1000000);
	BankAccount Lendee(100000);
	
	cout << fixed << setprecision(2);
	cout << "The lender has $" << Lender.getBalance() << endl;
	cout << "The lendee has $" << Lendee.getBalance() << endl << endl;

	//Create a contract between the two individuals and invoke it
	Contract loan(Lender, Lendee);
	loan.invokeContract(10000, 1.1);

	cout << "The contract has been fulfilled and terminated!" << endl << endl;
	cout << "The lender has $" << Lender.getBalance() << endl;
	cout << "The lendee has $" << Lendee.getBalance() << endl;

	return 0;
}
