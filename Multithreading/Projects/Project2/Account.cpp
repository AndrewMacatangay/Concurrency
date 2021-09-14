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

char* Account::communicateWithClient(string message, char* cStrBuffer, int FD)
{
	send(FD, message.c_str(), message.size() + 1, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	return cStrBuffer;
}

void Account::updateUserFile()
{
	//Launch new thread to create a new file and update the old file
	fstream userFile(".//UserData//" + username + ".csv", fstream::trunc | fstream::out);

	userFile << portfolio.size() << "," << balance << endl;

	for(pair<string, int> stock : portfolio)
		userFile << stock.first << "," << stock.second << endl;

	userFile.close();
}

string Account::registerAccount(int FD)
{
	string buffer, uBuffer, pBuffer;
	char cStrBuffer[4096];
	fstream accounts;

	createUsernamesCSV(accounts, buffer);
	unsigned int numberOfEntries = stoi(buffer);

	uBuffer = communicateWithClient("Enter username: ", cStrBuffer, FD);

	for(int x = numberOfEntries; x; x--)
	{
		getline(accounts, buffer, ',');
	
		if(buffer == uBuffer)
			return "Username already exists!\n";
		
		getline(accounts, buffer);
	}

	pBuffer = communicateWithClient("Enter password: ", cStrBuffer, FD);

	accounts << uBuffer << "," << pBuffer << "\n";
	accounts.clear();
	accounts.seekg(0);
	accounts << numberOfEntries + 1 << "\n";
	accounts.close();

	username = uBuffer;
	password = pBuffer;
	
	fstream userFile;
	
	//Add the file if it doesn't exist and close it
	userFile.open(".//UserData//" + username + ".csv", fstream::app);
	userFile.close();

	//Open the file for I/O
	userFile.open(".//UserData//" + username + ".csv", fstream::in | fstream::out);
	
	userFile << 0 << "," << 25000.00 << endl;
	userFile.close();
	
	return "Account created!\n";
}

string Account::loginAccount(int FD)
{
	string buffer, uBuffer, pBuffer;
	char cStrBuffer[4096];
	fstream accounts;

	createUsernamesCSV(accounts, buffer);
	unsigned int numberOfEntries = stoi(buffer);

	uBuffer = communicateWithClient("Enter username: ", cStrBuffer, FD);

	for(int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		
		if(temp == uBuffer)
		{
			pBuffer = communicateWithClient("Enter password: ", cStrBuffer, FD);
			if(pBuffer == buffer)
			{
				//Load account details
				isLoggedIn = 1;
				username = uBuffer;
				password = pBuffer;

				fstream userFile(".//UserData//" + username + ".csv");
				getline(userFile, buffer, ',');
				getline(userFile, buffer);
				balance = stod(buffer);

				for(string value; getline(userFile, buffer, ','); portfolio[buffer] = stoi(value))
					getline(userFile, value);

				return "Logged in!\n";
			}
			else
				return "Wrong password!\n";
		}
	}

	return "Username does not exist!\n";
}

string Account::logoutAccount()
{
	username = password = balance = isLoggedIn = 0;
	portfolio.clear();
	return "Logged out!\n";
}

string Account::buy(int FD)
{
	if(!isLoggedIn)
		return "Please log in first!\n";

	string ticker, amount;
	char cStrBuffer[4096];

	ticker = communicateWithClient("Ticker: ", cStrBuffer, FD);
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	amount = communicateWithClient("Amount: ", cStrBuffer, FD);
	
	//Check if NaN or negative amount
	if (stoi(amount) <= 0)
		return "Invalid amount entered!\n";

	string price = fetchData(ticker, 5);
	if (price.find("Error") != string::npos)
		return price;
	string rawPrice = price;
	rawPrice.erase(remove(rawPrice.begin(), rawPrice.end(), ','), rawPrice.end());

	if (balance < stod(rawPrice) * stoi(amount))
		return "Not enough funds!\n";

	balance -= stod(rawPrice) * stoi(amount);

	portfolio[ticker] += stoi(amount);

	updateUserFile();

	return "Bought " + amount + " " + ticker + " at $" + price + " each!\n" +
	       "Total: $" + to_string(stod(rawPrice) * stoi(amount)) + "\n" +
	       "Balance: $" + to_string(balance) + "\n";
}

string Account::sell(int FD)
{
	if(!isLoggedIn)
		return "Please log in first!\n";

	string ticker, amount;
	char cStrBuffer[4096];

	ticker = communicateWithClient("Ticker: ", cStrBuffer, FD);
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	amount = communicateWithClient("Amount: ", cStrBuffer, FD);

	string price = fetchData(ticker, 5);
	if(price.find("Error") != string::npos)
		return price;
	string rawPrice = price;
	rawPrice.erase(remove(rawPrice.begin(), rawPrice.end(), ','), rawPrice.end());

	if (stoi(amount) > portfolio[ticker])
		return "You do not own enough stock!\n";
	
	balance += stod(rawPrice) * stoi(amount);

	portfolio[ticker] -= stoi(amount);
	
	if(portfolio[ticker] == 0)
		portfolio.erase(ticker);

	updateUserFile();

	return "Sold " + amount + " " + ticker + " at $" + price + " each!\n" + 
		   "Total: $" + to_string(stod(rawPrice) * stoi(amount)) + "\n" +
		   "Balance: $" + to_string(balance) + "\n";
}

string Account::getUsername()
{
	return username;
}

bool Account::loggedIn()
{
	return isLoggedIn;
}
