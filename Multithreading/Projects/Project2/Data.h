class Data
{
	private:
	string ticker, JSON, buffer;
	bool isValidTicker;
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
			buffer.substr(0, buffer.find(','));
			buffer[buffer.size() - 2] == '.' && &(buffer += '0');
		}

		return buffer;

	}

	public:
	Data(string ticker, string curlBuffer) : ticker(ticker), JSON(curlBuffer)
	{
		isValidTicker = JSON.find("\"result\":[]") == string::npos;
	}
	
	string getBasicInformation()
	{
		if (!isValidTicker)
			return "Ticker symbol not found!";

		string padding(ticker.size() + 2, ' ');
		string name = getAttribute("shortName");
		string exchangeName = getAttribute("fullExchangeName");

		return ticker + ": " + name + '\n'
		              + padding + exchangeName;
	}

	string getPrice()
	{
		return ticker + ": " + getAttribute("regularMarketPrice");
	}
};
