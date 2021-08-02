#include <iostream>
#include <netinet/in.h> //https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
#include <sys/socket.h> //https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
#include <unistd.h>
using namespace std;

int main()
{
	//Create a socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}
	
	int reuseTrue = 1;
	int retVal = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuseTrue, sizeof(reuseTrue));
	if (retVal < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	bind(serverSocket, (struct sockaddr*) &addr, sizeof(addr));

	retVal = listen(serverSocket, 10);
	if (retVal < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

//	struct sockaddr_in remote_Addr;
	int sockLen = sizeof(addr);
	int socket = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*)&sockLen);
	if (socket < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	char buf[4096];

	/*while(1)
	{
		//int received = recv(socket, buf, 4096, 0);
		int received = read(socket, buf, 4096);
		send(socket, buf, received + 1, 0);
		cout << "Looped!" << endl;
	}*/

	read(socket, buf, 4096);
	printf("%s\n", buf);
	send(socket, "Hello from Server", 18, 0);
	printf("Hello sent from Server\n");

	close(serverSocket);

	return 0;
}
