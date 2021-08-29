#include "Account.h"

Account::Account() : isLoggedIn(0) { }

bool Account::loginAccount(int FD)
{
	string buffer;
	
	fstream accounts;
	accounts.open("Usernames.csv", fstream::app);
	accounts.close();

	accounts.open("Usernames.csv", fstream::in | fstream::out);

	if (!getline(accounts, buffer))
	{
		accounts.clear();
		accounts.seekg(0);
		accounts << 0 << "\n";
		accounts.seekg(0);
		getline(accounts, buffer);
	}

	unsigned int numberOfEntries = stoi(buffer);

	string username1, password1;
	char cStrBuffer[4096];
	//cout << "Enter username: ";
	send(FD, "Enter username: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	//cin >> username;
	username1 = cStrBuffer;
	//cout << "Test: " << username1 << endl;

	for(int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		
		if(temp == username1)
		{
			cout << "Enter password: ";
			cin >> password1;

			if(password1 == buffer)
			{
				cout << "Logged in!\n" << endl;
				isLoggedIn = 1;
				username = username1;
				password = password1;
				return 1;
			}
			else
			{
				cout << "Wrong password!\n";
			}
		}
	}

	cout << "Username does not exist!" << endl;
	return 0;
}

bool Account::registerAccount()
{
	return 1;
}
