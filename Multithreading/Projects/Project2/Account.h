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
	public:
		Account();
		bool loginAccount();
		bool registerAccount();
};

#endif
