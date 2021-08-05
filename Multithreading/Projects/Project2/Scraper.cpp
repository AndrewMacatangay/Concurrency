#include <algorithm>
#include <chrono>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <thread>
using namespace std;

int curlWriter(char* data, int size, int nmemb, string* buffer)
{
	if (buffer)
		buffer->append(data, size * nmemb);

	return buffer ? size * nmemb : 0;
}

void parsePrice(string ticker)
{
	CURL* curl = curl_easy_init();
	string URL = "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&symbols=" + ticker;
	
	for(string curlBuffer; 1; curlBuffer.clear())
	{
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_perform(curl);

		int priceIndex = curlBuffer.find("regularMarketPrice");
		string temp = curlBuffer.substr(priceIndex + 20);
		cout << ticker << ": " << temp.substr(0, temp.find(',')) << endl;
	
		this_thread::sleep_for(1s);
	}
}

int main()
{
	string ticker;
	cout << "Input ticker symbol: ";
	cin >> ticker;
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);

	parsePrice(ticker);
	
	return 0;
}
