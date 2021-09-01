#include "Account.h"

Account::Account() : isLoggedIn(0) { }

void Account::createUsernamesCSV(fstream& accounts, string& buffer)
{
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
}

string Account::registerAccount(int FD)
{
	string buffer, uBuffer, pBuffer;
	char cStrBuffer[4096];
	fstream accounts;

	createUsernamesCSV(accounts, buffer);
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
	string buffer, uBuffer, pBuffer;
	char cStrBuffer[4096];
	fstream accounts;

	createUsernamesCSV(accounts, buffer);
	unsigned int numberOfEntries = stoi(buffer);

	send(FD, "Enter username: ", 17, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	uBuffer = cStrBuffer;

	for(int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		
		if(temp == uBuffer)
		{
			send(FD, "Enter password: ", 17, 0);
			memset(cStrBuffer, 0, 4096);
			read(FD, cStrBuffer, 4096);
			pBuffer = cStrBuffer;
			if(pBuffer == buffer)
			{
				isLoggedIn = 1;
				username = uBuffer;
				password = pBuffer;
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
