#include "Account.h"

Account::Account() : isLoggedIn(0) { }

string Account::registerAccount(int FD)
{
	string buffer, uBuffer, pBuffer;
	char cStrBuffer[4096];
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

	send(FD, "Enter username: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	uBuffer = cStrBuffer;

	for(int x = numberOfEntries; x; x--)
	{
		getline(accounts, buffer, ',');
	
		if(buffer == uBuffer)
			return "Username already exists!\n";
		
		getline(accounts, buffer);
	}

	send(FD, "Enter password: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	pBuffer = cStrBuffer;

	accounts << uBuffer << "," << pBuffer << "\n";
	accounts.clear();
	accounts.seekg(0);
	accounts << numberOfEntries + 1 << "\n";
	accounts.close();

	username = uBuffer;
	password = pBuffer;

	return "Account created!\n";
}

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
	send(FD, "Enter username: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	username1 = cStrBuffer;

	for(int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		
		if(temp == username1)
		{
			send(FD, "Enter password: ", 17, 0);
			memset(cStrBuffer, 0, 4096);
			read(FD, cStrBuffer, 4096);
			password1 = cStrBuffer;
			if(password1 == buffer)
			{
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
