#include <iostream>
#include <curl/curl.h>
#include <string>
#include <algorithm>
#include <cctype>
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

int main()
{
	string ticker;
	cout << "Input ticker symbol: ";
	cin >> ticker;
	transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
	string URL = "https://finance.yahoo.com/quote/" + ticker;
	string curlBuffer;

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_perform(curl);

	//int priceIndex = curlBuffer.find("<span class=\"Trsdu(0.3s) Fw(b) Fz(36px) Mb(-4px) D(ib)\" data-reactid=\"49\">");
	
	//cout << curlBuffer.substr(priceIndex + 74, 6) << endl;
	int priceIndex = curlBuffer.find("currentPrice");
	cout << curlBuffer.substr(priceIndex - 1, 50) << endl;
	return 0;
}
