#include "Account.h"

Account::Account() : isLoggedIn(0) { }

string Account::loginAccount(int FD)
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
				//cout << "Logged in!\n" << endl;
				isLoggedIn = 1;
				username = username1;
				password = password1;
				return "Logged in!\n";
			}
			else
			{
				return "Wrong password!\n";
			}
		}
	}

	return "Username does not exist!\n";
}

string Account::registerAccount(int FD)
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
	cout << "Test: " << username1 << endl;

	for(int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		
		if(temp == username1)
			return "Username already exists!\n";
	}

	send(FD, "Enter password: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	password1 = cStrBuffer;
	cout << username1 << "," << password1 << endl;
	accounts << username1 << "," << password1 << "\n";
	accounts.clear();
	accounts.seekg(0);
	accounts << numberOfEntries + 1 << "\n";
	accounts.close();

	return "Account created!\n";
}
