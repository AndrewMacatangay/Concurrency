#include <arpa/inet.h>
#include <cstring>
#include <iostream>
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
		cout << "Enter a message: ";
		cin.getline(buffer, 4096);

		if (!strlen(buffer))
			continue;

		send(clientSocket, buffer, strlen(buffer), 0);
		read(clientSocket, buffer, 4096);
		cout << "Server: " << buffer << endl;
	}

	return 0;
}
