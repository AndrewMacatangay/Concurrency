#include "Libraries.h"
#include "Account.h"
int curlWriter(char* data, int size, int nmemb, string* buffer);
string fetchData(string ticker, int fetchType);

//Communicates with the accepted connection indefintely. Once a command or 
//ticker symbol is accepted, the appropriate information will be sent to
//the client
void communicate(int FD, int connection)
{
	//Make Account instance here, even if the client doesn't log in or register
	Account clientAccount;
	
	//Send opening message to the client
	string buffer = "Welcome to the stock market query! Type 'help' for commands\n\nEnter a command: ";
	send(FD, buffer.c_str(), buffer.size(), 0);
	
	for (char cStrBuffer[4096] = {1}; 1;)
	{
		//Pre-load username
		string username = "";
		if (clientAccount.loggedIn())
			username = " (" + clientAccount.getUsername() + ")";

		//Clear the buffer and read the input. Store the input
		//into a string for ease of processing
		memset(cStrBuffer, 0, 4096);
		read(FD, cStrBuffer, 4096);
		string query = cStrBuffer;

		//If a client disconnects, the loop continues for some reason,
		//and buffer[0] eventually becomes '\0'. This may need to be
		//revised in the future
		if (query == "help")
		{
			string padding(10, ' ');
			cout << "Client " << connection << username << ": " << query << "\n\n";
			buffer = "Commands: <ticker>\n"
				 + padding + "<ticker> today\n"
				 + padding + "<ticker> day averages\n"
				 + padding + "<ticker> volumes\n"
				 + padding + "<ticker> year\n"
				 + padding + "login\n" 
				 + padding + "register\n"
				 + padding + "logout\n"
				 + padding + "buy\n"
				 + padding + "sell\n";
		}
		//Fetch the data and store it into the buffer. If the
		//ticker symbol is valid, print the information on the
		//server side for bookkeeping
		else if (query.size())
		{
			size_t start = query.find(" ");
			size_t size = query.size();

			//Check if commands are valid here. If they are, call fetchData()
			if      (query.find(" today") != query.npos && size - 6 == start)
				buffer = fetchData(cStrBuffer, 1);
			else if (query.find(" day averages") != query.npos && size - 13 == start)
				buffer = fetchData(cStrBuffer, 2);
			else if (query.find(" volumes") != query.npos && size - 8 == start)
				buffer = fetchData(cStrBuffer, 3);
			else if (query.find(" year") != query.npos && size - 5 == start)
				buffer = fetchData(cStrBuffer, 4);
			else if (query == "login")
				buffer = clientAccount.loginAccount(FD);
			else if (query == "register")
				buffer = clientAccount.registerAccount(FD);
			else if (query == "logout")
				buffer = clientAccount.logoutAccount();
			//Error checking for string later
			else if (query == "buy")
				buffer = clientAccount.buy(FD);
			else
				buffer = fetchData(cStrBuffer, 0);

			if (buffer.find("Error") == string::npos)
			{
				cout << "Client " << connection << username << ": " << query << "\n"
				     << buffer << "\n";
			}
		}
		else
			{ cout << "Client " << connection << " disconnected!" << "\n\n"; return; }
		
		//Send the information to the client
		strncpy(cStrBuffer, buffer.c_str(), 4096);
		string fullMessage = string(cStrBuffer) + "\nEnter a command: ";
		send(FD, fullMessage.c_str(), fullMessage.size(), 0);
	}
}

//Indefintely accepts connections and launches another thread to communicate
//to the accepted connection
void acceptConnections(int serverSocket, sockaddr_in& addr, int& sockLen)
{
	for (int FD, connection = 1; 1; connection++)
	{
		if ((FD = accept(serverSocket, (struct sockaddr*) &addr, (socklen_t*) &sockLen)) < 0)
			{ cerr << "Connection failed!\n"; return; }

		cout << "Client " << connection << " connected!\n\n";
		thread(communicate, FD, connection).detach();
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

	//Create a thread of execution that handles all incoming connections
	//and requests from those connections
	thread(acceptConnections, serverSocket, ref(addr), ref(sockLen)).detach();
	
	//Let the server run indefinitely
	while(1);
	close(serverSocket);

	return 0;
}
