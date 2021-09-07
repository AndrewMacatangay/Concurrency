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
	if (!fetchType && ticker.find(' ') != ticker.npos)
		return "Error: Invalid Query\n";

	//Extract the ticker symbol from the rest of the query
	ticker = ticker.substr(0, ticker.find(' '));

	//Convert the ticker symbol to all uppercase letters
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
		return "Error: CURL Failed!\n";

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
	else if (fetchType == 5) returnBuffer = stockData.getPrice();

	return returnBuffer;
}
