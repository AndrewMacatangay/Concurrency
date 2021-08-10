class Data
{
	private:
	string ticker, JSON, buffer;
	bool isValidTicker, isCrypto;
	size_t attributeIndex;

	string getAttribute(string attribute)
	{
		attributeIndex = JSON.find(attribute);
		if (attributeIndex == JSON.npos)
			return "Ticker symbol not found!";
		
		buffer = JSON.substr(attributeIndex + attribute.size() + 2);

		//At this point we have either a string or a double that we need to reformat
		if(buffer[0] == '"')
		{
			buffer = buffer.substr(1);
			buffer = buffer.substr(0, buffer.find('"'));
		}
		else
		{
			buffer = buffer.substr(0, buffer.find(','));
			buffer[buffer.size() - 2] == '.' && &(buffer += '0');
		}

		return buffer;

	}

	//Let's add a loop for these
	//Market cap doesnt have a decimal and has more than 1 comma
	string formatNumber(string fund)
	{
		size_t decimalIndex = fund.find('.');
		for (decimalIndex == string::npos && (decimalIndex = fund.size()); decimalIndex > 3; decimalIndex -= 3)
			fund.insert(decimalIndex - 3, ",");
		return fund;
	}

	public:

	//Any other error checking for valid tickers must be included here
	Data(string ticker, string curlBuffer) : ticker(ticker), JSON(curlBuffer)
	{
		isValidTicker = JSON.find("\"result\":[]") == string::npos &&
				JSON.find("regularMarketPrice") != string::npos;
		isCrypto = JSON.find("CRYPTOCURRENCY") != string::npos;
	}
	
	string getBasicInformation()
	{
		if (!isValidTicker)
			return "Ticker symbol not found!";

		string padding(ticker.size() + 2, ' ');

		string name = getAttribute("shortName");
		string exchangeName = getAttribute(isCrypto ? "quoteSourceName" : "fullExchangeName");
		
		string marketPrice = formatNumber(getAttribute("regularMarketPrice"));
		string marketChange = formatNumber(getAttribute("regularMarketChange"));
		//Deal with dollar sign here
		       marketChange.insert((marketChange[0] == '-' ? 1 : 0), "$");

		string marketCap = formatNumber(getAttribute("marketCap"));

		string marketLow = formatNumber(getAttribute("regularMarketDayLow"));
		string marketHigh = formatNumber(getAttribute("regularMarketDayHigh"));

		string bid = formatNumber(getAttribute("bid"));
		string ask = formatNumber(getAttribute("ask"));

		return ticker + ": " + name + " (" + exchangeName + ")\n"
			      + padding + "$" + marketPrice + " (" + marketChange + ")" + "\n"
			      + padding + "Market Cap: $" + marketCap + "\n"
			      + padding + "Range: [$" + marketLow + ", $" + marketHigh + "]\n"
			      + padding + "Bid/Ask: <$" + bid + ", $" + ask + ">";
	}

	string getPrice()
	{
		return ticker + ": " + getAttribute("regularMarketPrice");
	}
};
