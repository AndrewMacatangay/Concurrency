#include "Libraries.h"
#include "Data.h"

//Mutex needed for race conditions on cout
//Utility function needed for curl_easy_setopt()
int curlWriter(char* data, int size, int nmemb, string* buffer)
{
	return buffer && &(buffer->append(data, size * nmemb)) ? size * nmemb : 0;
}

//Uses cURL to fetch data from Yahoo Finance
string fetchData(string ticker, int fetchType)
{
	//Catch any invalid queries here: ("tsla lksmd")
	if (!fetchType && ticker.find(' ') != string::npos)
		return "Error: Invalid Query";

	//Extract the ticker symbol from the rest of the query
	ticker = ticker.substr(0, ticker.find(' '));

	//Convert the ticker symbol to all uppercase letters
	//Might be an issue with adding commas to the ticker
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);

	//Declare and initialize the cURL object and store the JSON string
	//in "curlBuffer"
	CURL* curl = curl_easy_init();
	string URL = "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&symbols=" + ticker;
	string curlBuffer;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	if (curl_easy_perform(curl))
		return "Error: CURL Failed!";

	//Create a new data object which will parse the JSON string
	//based on the member function called
	Data stockData(ticker, curlBuffer);
	string returnBuffer;

	//Unecessary copying here
	if      (fetchType == 0) returnBuffer = stockData.getBasicInformation();
	else if (fetchType == 1) returnBuffer = stockData.getTodaysInformation();
	else if (fetchType == 2) returnBuffer = stockData.getDayAverages();
	else if (fetchType == 3) returnBuffer = stockData.getVolumes();
	else if (fetchType == 4) returnBuffer = stockData.getYear();

	return returnBuffer;
}

//Communicates with the accepted connection indefintely. Once a command or 
//ticker symbol is accepted, the appropriate information will be sent to
//the client
void communicate(int FD, int connection)
{
	string response;
	
	//Send opening message to the client
	string buffer = "Welcome to the stock market query! Type 'help' for commands";
	send(FD, buffer.c_str(), buffer.size() + 1, 0);
	
	for (char cStrBuffer[4096] = {1}; 1; )
	{
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
			cout << "Client " << connection << ": " << query << "\n\n";
			buffer = "Commands: <ticker>\n"
				 + padding + "<ticker> today\n"
				 + padding + "<ticker> day averages\n"
				 + padding + "<ticker> volumes\n"
				 + padding + "<ticker> year";
			strncpy(cStrBuffer, buffer.c_str(), 4096);
		}
		//Fetch the data and store it into the buffer. If the
		//ticker symbol is valid, print the information on the
		//server side for bookkeeping
		else if (query.size())
		{
			//Deal with cases here - check if "today" in query
			if (query.find("today") != string::npos)
				strncpy(cStrBuffer, fetchData(cStrBuffer, 1).c_str(), 4096);
			else if (query.find("day averages") != string::npos)
				strncpy(cStrBuffer, fetchData(cStrBuffer, 2).c_str(), 4096);
			else if (query.find("volumes") != string::npos)
				strncpy(cStrBuffer, fetchData(cStrBuffer, 3).c_str(), 4096);
			else if (query.find("year") != string::npos)
				strncpy(cStrBuffer, fetchData(cStrBuffer, 4).c_str(), 4096);
			else
				strncpy(cStrBuffer, fetchData(cStrBuffer, 0).c_str(), 4096);
			
			if (string(cStrBuffer).find("Error") == string::npos)
				cout << "Client " << connection << ": " << query << "\n"
				     << cStrBuffer << "\n\n";
		}
		else
			{ cout << "Client " << connection << " disconnected!" << "\n\n"; return; }
		
		//Send the information to the client
		send(FD, cStrBuffer, strlen(cStrBuffer), 0);
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
