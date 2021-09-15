#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "Libraries.h"
#include "Data.h"
string fetchData(string ticker, int fetchType);

class Account
{
	private:
		string username, password;
		double balance;
		map<string, int> portfolio;
		bool isLoggedIn;
		void createUsernamesCSV(fstream&, string&);
		char* communicateWithClient(string message, int FD);
		void updateUserFile();
		char cStrBuffer[4096];

	public:
		Account();
		string registerAccount(int FD);
		string loginAccount(int FD);
		string logoutAccount();
		string buy(int FD);
		string sell(int FD);
		string transaction(int FD, int transactionType);
		bool loggedIn();
		string getUsername();
};

#endif
