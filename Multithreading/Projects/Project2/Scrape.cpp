#include <iostream>
#include <curl/curl.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <thread>
#include <chrono>
using namespace std;

int curlWriter(char* data, int size, int nmemb, string* buffer)
{
	int result = 0;

	if (buffer)
	{
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}

	return result;
}

void parsePrice(string curlBuffer, string URL, string ticker)
{
	while(1)
	{
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_perform(curl);
		curl_easy_reset(curl);
		//int priceIndex = curlBuffer.find("currentPrice");
		int priceIndex = curlBuffer.find("regularMarketPrice");
		//cout << curlBuffer.substr(priceIndex - 1, 50) << endl;
		string temp = curlBuffer.substr(priceIndex + 20);
		cout << ticker << ": " << temp.substr(0, temp.find(',')) << endl;
		this_thread::sleep_for(1s);
		curlBuffer.clear();
	}
}

int main()
{
	string ticker;
	cout << "Input ticker symbol: ";
	cin >> ticker;
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	//string URL = "https://finance.yahoo.com/quote/" + ticker;
	string URL = "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&symbols=" + ticker;
	string curlBuffer;

	//CURL* curl = curl_easy_init();
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	//curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	//curl_easy_perform(curl);
	parsePrice(curlBuffer, URL, ticker);
	//int priceIndex = curlBuffer.find("<span class=\"Trsdu(0.3s) Fw(b) Fz(36px) Mb(-4px) D(ib)\" data-reactid=\"49\">");
	
	//cout << curlBuffer.substr(priceIndex + 74, 6) << endl;
	//int priceIndex = curlBuffer.find("currentPrice");
	//cout << curlBuffer.substr(priceIndex - 1, 50) << endl;
	return 0;
}
