#include <algorithm>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
using namespace std;
#include "Data.h"

//Mutex needed for race conditions on cout

int curlWriter(char* data, int size, int nmemb, string* buffer)
{
	return buffer && &(buffer->append(data, size * nmemb)) ? size * nmemb : 0;
}

string fetchData(string ticker)
{
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	//Might be an issue with adding commas to the ticker

	CURL* curl = curl_easy_init();
	string URL = "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&symbols=" + ticker;
	string curlBuffer;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	if (curl_easy_perform(curl))
		return "Error loading ticker symbol!";

	Data stockData(ticker, curlBuffer);
	return stockData.getBasicInformation();
}

void communicate(int connection, int connectionNumber)
{
	string response;
	for (char buffer[4096] = {1}; 1; )
	{
		//Once you're done using the information from the buffer, clear it
		memset(buffer, 0, 4096);
		read(connection, buffer, 4096);
		string query = buffer;
		if (buffer[0])
		{
			strncpy(buffer, fetchData(buffer).c_str(), 4096);
			if (string(buffer).find("Ticker symbol not found!") == string::npos)
				cout << "Client " << connectionNumber << ": " << query << "\n" << buffer << "\n\n";
		}
		else
			{ cout << "Client " << connectionNumber << " disconnected!" << "\n\n"; return; }
		send(connection, buffer, strlen(buffer), 0);
	}
}

void acceptConnections(int serverSocket, sockaddr_in& addr, int& sockLen)
{
	//Connection number should be atomic?
	for (int connection, connectionNumber = 1; 1; connectionNumber++)
	{
		if ((connection = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
			{ cerr << "Connection failed!\n"; return; }
		cout << "Client " << connectionNumber << " connected!\n\n";
		thread(communicate, connection, connectionNumber).detach();
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
