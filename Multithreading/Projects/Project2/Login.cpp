#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

int main()
{
	bool newAccount = 1;
	string username, buffer;
	fstream accounts;
	
	accounts.open("Usernames.csv", fstream::out | fstream::app);
	accounts.close();
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
	
	cout << "Enter a username: ";
	cin >> username;

	//Loop through the CSV files and return if the name already exists
	for (int x = numberOfEntries; x; x--)
	{
		getline(accounts, buffer);
		if (buffer == username)
		{
			cout << "Logged in!" << endl;
			newAccount = 0;
			break;
		}
	}

	if (newAccount)
	{
		//Since the name does not exist in the CSV file, add it
		accounts << username << "\n";
	
		//Clear the file stream flags, update the number of names,
		//and close the file.
		accounts.clear();
		accounts.seekp(0);
		accounts << numberOfEntries + 1;

		accounts.close();
	}

	fstream userFile;
	userFile.open(".//UserData//" + username + ".csv", fstream::out | fstream::app);
	userFile.close();
	userFile.open(".//UserData//" + username + ".csv", fstream::in | fstream::out);

	if (newAccount)
	{
		userFile << 0 << endl;
		userFile.seekg(0);
	}

	bool foundTicker = 0;
	string ticker;
	getline(userFile, buffer);
	unsigned int numberOfTickers = stoi(buffer);

	cout << "Enter a ticker symbol: ";
	cin >> ticker;

	for(int x = numberOfTickers; x; x--)
	{
		getline(userFile, buffer);
		if (ticker == buffer)
		{
			foundTicker = 1;
			cout << "Ticker found!" << endl;
		}
	}
	if (!foundTicker)
	{
		cout << "Ticker not found" << endl;
			
		userFile << ticker << endl;
		userFile.seekg(0);
		userFile << numberOfTickers + 1;
	}

	userFile.close();

	return 0;
}
