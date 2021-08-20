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

	fstream accounts("Accounts.csv");
	cout << "Enter a name: ";
	cin >> account.firstName;

	if(accounts.is_open())
		cout << "We are open!" << endl;
	
	while(getline(accounts, buffer))
		cout << buffer << endl;

	accounts << account.firstName << "\n";
	accounts.close();

	return 0;
}
