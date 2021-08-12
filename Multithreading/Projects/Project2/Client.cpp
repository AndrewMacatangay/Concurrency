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
	//Initialize client socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {AF_INET, htons(8080)};
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	//Connect to server
	if(connect(clientSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	//Wait for the opening message, then print it
	char buffer[4096] = {1};
	read(clientSocket, buffer, 4096);
	cout << buffer << endl << endl;
	
	//Ask the user to enter a ticker symbol and wait for a response from
	//the server if the input was valid. Do this indefinitely
	while (1)
	{
		//Store the input into cin and convert it to a string
		//for processing
		cout << "Enter a ticker symbol: ";
		cin.getline(buffer, 4096);
		string b2s(buffer);
		
		//If an escape character or all characters are whitespace
		//characters, the ticker is invalid
		if (b2s.find(27) != b2s.npos || all_of(b2s.begin(), b2s.end(), [](char c){ return isspace(c); }))
		{ cout << "Error: Invalid ticker symbol!\n\n"; continue; }

		//Now that the input is well-formatted, send it to the server
		//for processing. Clear the buffer, wait until the server sends
		//back information, and read it
		send(clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, 4096);
		read(clientSocket, buffer, 4096);
		cout << buffer << endl << endl;
	}

	return 0;
}
