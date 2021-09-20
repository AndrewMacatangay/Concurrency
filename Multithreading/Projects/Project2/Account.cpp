#include "Account.h"

Account::Account() : isLoggedIn(0) { }

//Create the CSV files containing the usernames, and print out 0
//indicating the number of usernames in the file
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

//And a message to the client with the given file descriptor and
//wait for the response
char* Account::communicateWithClient(string message, int FD)
{
	send(FD, message.c_str(), message.size() + 1, 0);
	memset(cStrBuffer, 0, 4096);
	read(FD, cStrBuffer, 4096);
	return cStrBuffer;
}

//Once a buy or sell transaction is made, we must update the user's
//file immediately so no data is lost
void Account::updateUserFile()
{
	//Launch new thread to create a new file and update the old file
	fstream userFile(".//UserData//" + username + ".csv", fstream::trunc | fstream::out);

	userFile << portfolio.size() << "," << balance << endl;

	for(pair<string, int> stock : portfolio)
		userFile << stock.first << "," << stock.second << endl;

	userFile.close();
}

//Create a new account
string Account::registerAccount(int FD)
{
	unique_lock<mutex> l(functionMutex);
	if(isLoggedIn)
		return "Please log out first!\n";

	string buffer, uBuffer, pBuffer;
	fstream accounts;

	//Create a new Usernames CSV if one does not exist
	//It will act as the root database for all user actions
	createUsernamesCSV(accounts, buffer);
	unsigned int numberOfEntries = stoi(buffer);

	uBuffer = communicateWithClient("Enter username: ", FD);

	//Loop through all the usernames to see if it exists
	for(int x = numberOfEntries; x; x--)
	{
		//Get the username
		getline(accounts, buffer, ',');

		if(buffer == uBuffer)
			return "Username already exists!\n";

		//Get the password, but don't do anything with it
		getline(accounts, buffer);
	}

	pBuffer = communicateWithClient("Enter password: ", FD);

	//Update the number of people in the Username CSV file
	accounts << uBuffer << "," << pBuffer << "\n";
	accounts.clear();
	accounts.seekg(0);
	accounts << numberOfEntries + 1 << "\n";
	accounts.close();

	fstream userFile;
	
	//Add the user file if it doesn't exist and close it
	userFile.open(".//UserData//" + username + ".csv", fstream::app);
	userFile.close();

	//Open the file for I/O and start with $25K
	userFile.open(".//UserData//" + username + ".csv", fstream::in | fstream::out);
	userFile << 0 << "," << 25000.00 << endl;
	userFile.close();
	
	return "Account created!\n";
}

//Log the user in so that they can buy/sell stocks
string Account::loginAccount(int FD)
{
	if (isLoggedIn)
		return "You are already logged in!\n";

	string buffer, uBuffer, pBuffer;
	fstream accounts;

	//Create a new Usernames CSV if one does not exist
	//It will act as the root databse for all user actions
	createUsernamesCSV(accounts, buffer);
	unsigned int numberOfEntries = stoi(buffer);

	uBuffer = communicateWithClient("Enter username: ", FD);

	//Loop through all the usernames to see if it exists
	for(int x = numberOfEntries; x; x--)
	{
		string temp;

		//Store the username and password
		getline(accounts, temp, ',');
		getline(accounts, buffer);

		if(temp == uBuffer)
		{
			pBuffer = communicateWithClient("Enter password: ", FD);
			if(pBuffer == buffer)
			{
				//Since the password matches, input all the user data
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

//Allows the user to buy or sell stocks
string Account::transaction(int FD, int transactionType)
{
	unique_lock<mutex> l(functionMutex);
	if(!isLoggedIn)
		return "Please log in first!\n";

	string ticker, amount;

	ticker = communicateWithClient("Ticker: ", FD);
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	amount = communicateWithClient("Amount: ", FD);
	
	//Check if NaN or negative amount
	if (stoi(amount) <= 0)
		return "Invalid amount entered!\n";

	//Get the current price from Yahoo Finance
	string price = fetchData(ticker, 5);
	if (price.find("Error") != string::npos)
		return price;

	//Remove any commas from the formatted price so we can
	//use arithmetic on the raw price
	string rawPrice = price;
	rawPrice.erase(remove(rawPrice.begin(), rawPrice.end(), ','), rawPrice.end());

	//Buying and selling errors
	if (transactionType && balance < stod(rawPrice) * stoi(amount))
		return "Not enough funds!\n";
	else if(!transactionType && stoi(amount) > portfolio[ticker])
		return "You do not own enough stock!\n";

	//Update the portfolio
	double cost = stod(rawPrice) * stoi(amount);
	balance += transactionType ? -cost : cost;

	portfolio[ticker] += transactionType ? stoi(amount) : -stoi(amount);
	
	if(!portfolio[ticker])
		portfolio.erase(ticker);

	updateUserFile();

	return (transactionType ? "Bought " : "Sold ") + amount + " " + ticker + " at $" + price + " each!\n" +
	       "Total: $" + to_string(stod(rawPrice) * stoi(amount)) + "\n" +
	       "Balance: $" + to_string(balance) + "\n";	
}

//Logs out a logged in account
string Account::logoutAccount()
{
	if (!isLoggedIn)
		return "You are not logged in!\n";

	username = password = balance = isLoggedIn = 0;
	portfolio.clear();
	return "Logged out!\n";
}

string Account::getUsername()
{
	return username;
}

string Account::getBalance()
{
	if (!isLoggedIn)
		return "You are not logged in!\n";
	return "Balance: " + to_string(balance) + "\n";
}

string Account::getPortfolio()
{
	if (!isLoggedIn)
		return "You are not logged in!\n";

	string buffer;
	for(pair<string, int> stock : portfolio)
		buffer += stock.first + " " + to_string(stock.second) + "\n";

	return buffer;
}

bool Account::loggedIn()
{
	return isLoggedIn;
}
