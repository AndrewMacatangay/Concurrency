#include "Libraries.h"

//The Data class is dedicated to receive and process a JSON string
class Data
{
	private:
		string ticker, JSON, buffer;
		bool isValidTicker, isCrypto;
		size_t attributeIndex;

		string getAttribute(string attribute);
		string formatNumber(string number);

	public:

		Data(string ticker, string curlBuffer);
		string getBasicInformation();
		string getTodaysInformation();
		string getDayAverages();
		string getVolumes();
		string getYear();
		string getPrice();
};
