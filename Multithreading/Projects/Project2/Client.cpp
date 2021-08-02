#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	char buffer[4096];

	struct sockaddr_in addr = {AF_INET, htons(8080)};
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	connect(clientSocket, (struct sockaddr*) &addr, sizeof(addr));

	send(clientSocket, "Hello from Client", 18, 0);
	cout << "Hello sent from Client" << endl;
	read(clientSocket, buffer, 4096);
	cout << buffer << endl;
	
	return 0;
}
