#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

struct Account
{
	string firstName;
	int ID;
	vector<pair<string, int>> portfolio;
};

int main()
{
	Account account;

	string buffer;

	fstream accounts("Accounts.csv", fstream::in | fstream::out);
	cout << "Enter a name: ";
	cin >> account.firstName;

	if(accounts.is_open())
		cout << "We are open!" << endl;
//	cout << accounts.good() << accounts.eof() << accounts.fail() << accounts.bad() << endl;

	int x;
	accounts >> x;
	while(x--)
	{
		cout << buffer << endl;
//		cout << accounts.good() << accounts.eof() << accounts.fail() << accounts.bad() << endl;
	}
	//0110
	cout << accounts.good() << accounts.eof() << accounts.fail() << accounts.bad() << endl;

	accounts << account.firstName << "\n";
	accounts.close();

	return 0;
}
