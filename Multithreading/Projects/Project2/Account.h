#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "Libraries.h"

class Account
{
	private:
		string username, password;
		double balance;
		vector<pair<string, int>> portfolio;
		bool isLoggedIn;
		void createUsernamesCSV(fstream&, string&);

	public:
		Account();
		string registerAccount(int FD);
		string loginAccount(int FD);
		string logoutAccount();
		bool loggedIn();
		string getUsername();
};

#endif
