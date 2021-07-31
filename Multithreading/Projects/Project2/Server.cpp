#include <iostream>
#include <fnmatch.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
using namespace std;

int main()
{
	//Create a socket
	int serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}
	
	int reuseTrue = 1;
	int retVal = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseTrue, sizeof(reuseTrue));
	if (retVal < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	struct sockaddr_in6 addr;
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(8080);
	addr.sin6_addr = in6addr_any;
	
	bind(serverSocket, (sockaddr*) &addr, sizeof(addr));

	retVal = listen(serverSocket, 10);
	if (retVal < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	struct sockaddr_in remote_Addr;
	unsigned int sockLen = sizeof(remote_Addr);
	int socket = accept(serverSocket, (struct sockaddr*) &remote_Addr, &sockLen);
	if (socket < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	char buf[4096];

	while(1)
	{
		int received = recv(socket, buf, 4096, 0);
		send(socket, buf, received + 1, 0);
		cout << "Looped!" << endl;
	}

	close(serverSocket);

	return 0;
}
