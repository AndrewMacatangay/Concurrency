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

	string temp;
	getline(accounts, temp);
	unsigned int entries = stoi(temp);

	for (int x = entries; x; x--)
	{
		getline(accounts, buffer);
		if (buffer == account.firstName)
		{
			cout << "Name already exists!" << endl;
			accounts.close();
			return 0;
		}
	}

	accounts << account.firstName << "\n";
	accounts.clear();
	accounts.seekg(0);
	accounts << entries + 1;
	accounts.close();

	return 0;
}
