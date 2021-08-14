//The Data class is dedicated to receive and process a JSON string
class Data
{
	private:
	string ticker, JSON, buffer;
	bool isValidTicker, isCrypto;
	size_t attributeIndex;

	//General purpose function used to parse data from a given attribute
	string getAttribute(string attribute)
	{
		//Find the index where the attribute begins. If the
		//attribute does not exist, return an error message
		attributeIndex = JSON.find(attribute);
		if (attributeIndex == JSON.npos)
			return "Error: Attribute Does Not Exist";
		
		//We want to slice the string from the beginning of the data
		//to the end of the JSON string for now
		buffer = JSON.substr(attributeIndex + attribute.size() + 2);

		//At this point we have either a string or a double that we need to reformat.
		//We have to slice the string such that the remaining attributes from the JSON
		//string that are not needed are pruned
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

	string formatNumber(string number)
	{
		//If the number is negative, we should remove the sign and 
		//insert it after adding the commas. 
		bool negative = number[0] == '-';
		negative && &(number = number.substr(1));

		//Find the index of the decimal or the end of the number to
		//find the index where we start making commas from
		size_t decimalIndex = number.find('.');
		decimalIndex == string::npos && (decimalIndex = number.size());

		//For every 3 digits, add a comma
		for ( ; decimalIndex > 3; decimalIndex -= 3)
			number.insert(decimalIndex - 3, ",");

		//If the number was negative, then insert the negative sign back 
		negative && &number.insert(0, "-");
		return number;
	}

	public:

	//Data constructor:
	//Any other error checking for valid tickers must be included here. Checks
	//will be done at the beginning of each member function to check if the
	//ticker symbol is valid to avoid extra computation
	Data(string ticker, string curlBuffer) : ticker(ticker), JSON(curlBuffer)
	{
		isValidTicker = JSON.find("\"result\":[]") == string::npos &&
				JSON.find("regularMarketPrice") != string::npos;
		isCrypto = JSON.find("CRYPTOCURRENCY") != string::npos;
	}

	//Executed when only the ticker symbol is given
	string getBasicInformation()
	{
		if (!isValidTicker)
			return "Error: Invalid Ticker Symbol";

		string padding(ticker.size() + 2, ' ');

		string name         = getAttribute("shortName");
		string exchangeName = getAttribute(isCrypto ? "quoteSourceName" : "fullExchangeName");
		string marketPrice  = formatNumber(getAttribute("regularMarketPrice"));

		string marketCap    = formatNumber(getAttribute("marketCap"));

		string coinSupply   = isCrypto ? formatNumber(getAttribute("circulatingSupply")) : "";

		return ticker + ": " + name + " (" + exchangeName + "): $" + marketPrice + "\n"
			      + padding + "Market Cap: $" + marketCap
			      + (isCrypto ? "\n" + padding + "Circulating Supply: " + coinSupply : "");
	}
	
	//Executed when "today" is included in the query
	string getTodaysInformation()
	{
		if (!isValidTicker)
			return "Error: Invalid Ticker Symbol";

		string padding(ticker.size() + 2, ' ');

		string marketPrice         = formatNumber(getAttribute("regularMarketPrice"));
		string marketChange        = formatNumber(getAttribute("regularMarketChange"));
		string marketChangePercent = formatNumber(getAttribute("regularMarketChangePercent"));

		string marketLow           = formatNumber(getAttribute("regularMarketDayLow"));
		string marketHigh          = formatNumber(getAttribute("regularMarketDayHigh"));

		string marketClose         = formatNumber(getAttribute("regularMarketPreviousClose"));
		string marketOpen          = formatNumber(getAttribute("regularMarketOpen"));

		string bid                 = formatNumber(getAttribute("bid"));
		string ask                 = formatNumber(getAttribute("ask"));

		//Any modications made to the attributes are done here
		marketChange.insert(0, marketChange[0] == '-' ? "" : "+");
		marketChange.insert(1, "$");
		marketChangePercent.insert(0, marketChangePercent[0] == '-' ? "" : "+");
		
		return ticker + ": $" + marketPrice + " (" + marketChange + ", " + marketChangePercent + "%)\n" 
			      + padding + "Range:      [$" + marketLow + ", $" + marketHigh + "]\n"
			      + padding + "Close/Open: {$" + marketClose + ", $" + marketOpen + "}"
			      + (!isCrypto ? "\n" + padding + "Bid/Ask:    <$" + bid + ", $" + ask + ">" : "");
	}

	//Executed when "day averages" is included in the query
	string getDayAverages()
	{	
		if (!isValidTicker)
			return "Error: Invalid Ticker Symbol";

		string padding(ticker.size() + 2, ' ');

		string marketPrice = formatNumber(getAttribute("regularMarketPrice"));

		string FDA         = formatNumber(getAttribute("fiftyDayAverage"));
		string FDAC        = formatNumber(getAttribute("fiftyDayAverageChange"));
		string FDACP       = formatNumber(getAttribute("fiftyDayAverageChangePercent"));

		string THDA        = formatNumber(getAttribute("twoHundredDayAverage"));
		string THDAC       = formatNumber(getAttribute("twoHundredDayAverageChange"));
		string THDACP      = formatNumber(getAttribute("twoHundredDayAverageChangePercent"));

		//Any modications made to the attributes are done here
		FDAC.insert(0, FDAC[0] == '-' ? "" : "+");
		FDAC.insert(1, "$");
		FDACP.insert(0, FDACP[0] == '-' ? "" : "+");
		THDAC.insert(0, THDAC[0] == '-' ? "" : "+");
		THDAC.insert(1, "$");
		THDACP.insert(0, THDACP[0] == '-' ? "" : "+");

		return ticker + ": $" + marketPrice + "\n"
		              + padding + "50 Day:  $" + FDA + " (" + FDAC + ", " + FDACP + "%)\n"
			      + padding + "200 Day: $" + THDA + " (" + THDAC + ", " + THDACP + "%)";
	}
	
	string getVolumes()
	{
		if (!isValidTicker)
			cout << "Error: Invalid Ticker Symbol";
		
		string padding(ticker.size() + 2, ' ');

		string marketPrice        = formatNumber(getAttribute("regularMarketPrice"));
	
		string marketVolume       = formatNumber(getAttribute("regularMarketVolume"));
		string averageDaily3Month = formatNumber(getAttribute("averageDailyVolume3Month"));
		string averageDaily10Day  = formatNumber(getAttribute("averageDailyVolume10Day"));

		return ticker + ": $" + marketPrice + "\n"
		              + padding + "Today's Volume:        " + marketVolume + "\n"
			      + padding + "Average Daily 3 Month: " + averageDaily3Month + "\n"
			      + padding + "Average Daily 10 Day:  " + averageDaily10Day;
	}

	string getPrice()
	{
		return ticker + ": " + getAttribute("regularMarketPrice");
	}
};
