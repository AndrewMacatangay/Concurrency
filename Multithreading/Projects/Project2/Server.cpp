#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
using namespace std;

void echo(int connection)
{
	char buffer[4096];
	while (buffer[0] != 'a')
	{
		//Once you're done using the information from the buffer, clear it
		memset(buffer, 0, 4096);
		int bytesReceived = read(connection, buffer, 4096);
		cout << "Client: " << buffer << endl;
		send(connection, buffer, bytesReceived + 1, 0);
	}
}

void acceptConnections(int serverSocket, sockaddr_in& addr, int& sockLen)
{
	for (int connection; 1; )
	{
		if ((connection = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
			{ cerr << "Connection failed!\n"; return; }
		cout << "Connection: " << connection << endl;
		thread(echo, connection).detach();
	}
}

int main()
{
	int serverSocket, opt = 1, sockLen;

	if (!(serverSocket = socket(AF_INET, SOCK_STREAM, 0)))
		{ cerr << "Socket could not be made!\n"; return 1; }

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		{ cerr << "Socket options failed!\n"; return 1; }

	struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
	sockLen = sizeof(addr);

	if (bind(serverSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		{ cerr << "Binding failed!\n"; return 1; }

	if (listen(serverSocket, 10) < 0)
		{ cerr << "Listening failed!\n"; return 1; }

	thread(acceptConnections, serverSocket, ref(addr), ref(sockLen)).detach();
	
	//Let the server run forever. Might change this to be some escape character
	while(1);

	close(serverSocket);

	return 0;
}
