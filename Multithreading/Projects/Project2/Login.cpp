#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

int main()
{
	string username, buffer;
	fstream accounts("Usernames.csv", fstream::in | fstream::out);
	
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
			cout << "Name already exists!" << endl;
			accounts.close();
			return 0;
		}
	}

	//Since the name does not exist in the CSV file, add it
	accounts << username << "\n";
	
	//Clear the file stream flags, update the number of names,
	//and close the file.
	accounts.clear();
	accounts.seekg(0);
	accounts << numberOfEntries + 1;

	accounts.close();

	fstream userFile(".//UserData//" + username + ".csv", fstream::out);
	userFile.close();

	return 0;
}
