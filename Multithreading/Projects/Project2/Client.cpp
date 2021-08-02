#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

int main()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	connect(clientSocket, (struct sockaddr*) &addr, sizeof(addr));

	char buffer[4096];
	send(clientSocket, "Hello from Client", 18, 0);
	cout << "Hello sent from Client" << endl;
	read(clientSocket, buffer, 4096);
	printf("%s\n", buffer);
	return 0;
}
