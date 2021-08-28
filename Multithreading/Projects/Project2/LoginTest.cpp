#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

struct userData
{
	int numberOfTickers;
	double balance;
	vector<string> tickers;
};

int createUsernamesCSV(fstream& accounts, bool& newAccount, string& username, string& password)
{
	//Decide whether to make a new account (register), or
	//log into an existing account
	string buffer;
	bool registering = 0;

	//Add the file if it doesn't exist and close it
	accounts.open("Usernames.csv", fstream::app);
	accounts.close();

	//Open up the file for I/O
	accounts.open("Usernames.csv", fstream::in | fstream::out);
	
	//If there is nothing to read in, let the number of entries be 0	
	if (!getline(accounts, buffer))
	{
		accounts.clear();
		accounts.seekg(0);
		accounts << 0 << "\n";
		accounts.seekg(0);
		getline(accounts, buffer);
	}

	unsigned int numberOfEntries = stoi(buffer);

	cout << "Would you like to login or register?" << endl;
	cin >> buffer;
	registering = buffer == "register";

	cout << "Enter a username: ";
	cin >> username;

	//Loop through the CSV files and return if the name already exists
	for (int x = numberOfEntries; x; x--)
	{
		string temp;
		getline(accounts, temp, ',');
		getline(accounts, buffer);
		if (temp == username)
		{
			if (registering)
			{
				cout << "Username already taken!" << endl;
				return 0;
			}

			cout << "Enter password: ";
			cin >> password;
			
			if (password == buffer)
			{
				cout << "Logged in!" << endl;
				newAccount = 0;
				break;
			}
			cout << "Wrong password" << endl;
			accounts.close();
			return 0;
		}
	}

	if (newAccount)
	{
		cout << "Enter password: ";
		cin >> password;
		//Since the name does not exist in the CSV file, add it
		accounts << username << "," << password << "\n";
	
		//Clear the file stream flags, update the number of names,
		//and close the file.
		accounts.clear();
		accounts.seekp(0);
		accounts << numberOfEntries + 1;

		accounts.close();
	}
	return 1;
}

void createUserFile(fstream& userFile, bool& newAccount, string& username, string& password)
{
	string buffer;
	userData data;

	//Add the file if it doesn't exist and close it
	userFile.open(".//UserData//" + username + ".csv", fstream::app);
	userFile.close();

	//Open the file for I/O
	userFile.open(".//UserData//" + username + ".csv", fstream::in | fstream::out);

	//If we have a new account, then set the default parameters
	if (newAccount)
	{
		userFile << 0 << "," << 25000.00 << endl;
		userFile.seekg(0);
	}

	bool foundTicker = 0;
	string ticker;

	getline(userFile, buffer, ',');
	data.numberOfTickers = stoi(buffer);

	getline(userFile, buffer);
	data.balance = stod(buffer);

	cout << "Enter a ticker symbol: ";
	cin >> ticker;

	//Push each existing ticker into the vector
	for(int x = data.numberOfTickers; x; x--)
	{
		getline(userFile, buffer);
		data.tickers.push_back(buffer);
	}

	//Check if the ticker entered by the user already exists
	for (string x : data.tickers)
		if (ticker == x)
			foundTicker = 1;

	//Add the ticker if the doesn't exist
	if (!foundTicker)
	{
		data.tickers.push_back(ticker);
		data.numberOfTickers++;
	}
	userFile.close();

	//Clear the file and output
	userFile.open(".//UserData//" + username + ".csv", fstream::out | fstream::trunc);

	//Enter the updated data into the file
	userFile << data.numberOfTickers << "," << data.balance << "\n";
	for (string x : data.tickers)
		userFile << x << "\n";

	userFile.close();
}

int main()
{
	bool newAccount = 1;
	string username, password;
	fstream accounts, userFile;

	if (createUsernamesCSV(accounts, newAccount, username, password))
		createUserFile(userFile, newAccount, username, password);

	return 0;
}
