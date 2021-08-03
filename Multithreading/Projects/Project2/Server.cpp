#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main()
{
	int serverSocket, opt = 1, sockLen, connection;
	char buffer[4096];

	if (!(serverSocket = socket(AF_INET, SOCK_STREAM, 0)))
	{ cerr << "Failed!" << endl; return 1; }

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{ cerr << "Failed!" << endl; return 1; }

	struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
	sockLen = sizeof(addr);

	if (bind(serverSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	if (listen(serverSocket, 10) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	if ((connection = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	while (buffer[0] != 'a')
	{
		//Once you're done using the information from the buffer, clear it
		memset(buffer, 0, 4096);
		int bytesReceived = read(connection, buffer, 4096);
		cout << "Client: " << buffer << endl;
		send(connection, buffer, bytesReceived + 1, 0);
	}

	close(serverSocket);

	return 0;
}
