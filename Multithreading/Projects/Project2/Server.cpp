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

	if (listen(serverSocket,10) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	if ((connection = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
	{ cerr << "Failed!" << endl; return 1; }

	/*while(1)
	{
		//int received = recv(socket, buf, 4096, 0);
		int received = read(socket, buf, 4096);
		send(socket, buf, received + 1, 0);
		cout << "Looped!" << endl;
	}*/

	read(connection, buffer, 4096);
	cout << buffer << endl;
	send(connection, "Hello from Server", 18, 0);
	cout << "Hello sent from Server" << endl;

	close(serverSocket);

	return 0;
}
