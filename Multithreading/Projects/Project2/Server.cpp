#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main()
{
	int serverSocket;
	if (!(serverSocket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		cout << "Failed!" << endl;
		return 1;
	}

	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		cout << "Failed!" << endl;
		return 1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	if (listen(serverSocket,10) < 0)
	{
		cout << "Failed!" << endl;
		return 1;
	}

	int sockLen = sizeof(addr);
	int socket;
	if ((socket = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
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
