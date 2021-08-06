#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <locale>
#include <string>
#include <unistd.h>
using namespace std;

int main()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {AF_INET, htons(8080)};
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	if(connect(clientSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	for (char buffer[4096] = {1}; 1; )
	{
		memset(buffer, 0, 4096);
		cout << "Enter a ticker symbol: ";
		cin.getline(buffer, 4096);
		string b2s(buffer);

		if (b2s.find(27) != b2s.npos || all_of(b2s.begin(), b2s.end(), [](char c){ return isspace(c); }))
		{
			cout << "Invalid ticker symbol!" << endl << endl;
			continue;
		}

		send(clientSocket, buffer, strlen(buffer), 0);
		read(clientSocket, buffer, 4096);
		cout << buffer << endl << endl;
	}

	return 0;
}
