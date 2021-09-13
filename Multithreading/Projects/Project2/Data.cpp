#include "Data.h"

//General purpose function used to parse data from a given attribute
string Data::getAttribute(string attribute)
{
	//Find the index where the attribute begins. If the
	//attribute does not exist, return an error message
	
	attributeIndex = JSON.find(attribute);
	if (attributeIndex == JSON.npos)
		return "Error: Attribute Does Not Exist\n";
	
	//We want to slice the string from the beginning of the data
	//to the end of the JSON string for now
	buffer = JSON.substr(attributeIndex + attribute.size());

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

string Data::formatNumber(string number)
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

//Data constructor:
//Any other error checking for valid tickers must be included here. Checks
//will be done at the beginning of each member function to check if the
//ticker symbol is valid to avoid extra computation
Data::Data(string ticker, string curlBuffer) : ticker(ticker), JSON(curlBuffer)
{
	isValidTicker = JSON.find("\"result\":[]") == JSON.npos &&
			JSON.find("regularMarketPrice") != JSON.npos;

	isCrypto = JSON.find("CRYPTOCURRENCY") != JSON.npos;
}

//Executed when only the ticker symbol is given
string Data::getBasicInformation()
{
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";

	string padding(ticker.size() + 2, ' ');
	string name         = getAttribute("\"shortName\":");
	string exchangeName = getAttribute(isCrypto ? "\"quoteSourceName\":" : "\"fullExchangeName\":");
	string marketPrice  = formatNumber(getAttribute("\"regularMarketPrice\":"));
	string marketCap    = formatNumber(getAttribute("\"marketCap\":"));
	string coinSupply   = isCrypto ? formatNumber(getAttribute("\"circulatingSupply\":")) : "";

	return ticker + ": " + name + " (" + exchangeName + "): $" + marketPrice + "\n"
		      + padding + "Market Cap: $" + marketCap
		      + (isCrypto ? "\n" + padding + "Circulating Supply: " + coinSupply : "") + "\n";
}

//Executed when "today" is included in the query
string Data::getTodaysInformation()
{
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";

	string padding(ticker.size() + 2, ' ');
	string marketPrice         = formatNumber(getAttribute("\"regularMarketPrice\":"));
	string marketChange        = formatNumber(getAttribute("\"regularMarketChange\":"));
	string marketChangePercent = formatNumber(getAttribute("\"regularMarketChangePercent\":"));
	string marketLow           = formatNumber(getAttribute("\"regularMarketDayLow\":"));
	string marketHigh          = formatNumber(getAttribute("\"regularMarketDayHigh\":"));
	string marketClose         = formatNumber(getAttribute("\"regularMarketPreviousClose\":"));
	string marketOpen          = formatNumber(getAttribute("\"regularMarketOpen\":"));
	string bid                 = formatNumber(getAttribute("\"bid\":"));
	string ask                 = formatNumber(getAttribute("\"ask\":"));

	//Any modifications made to the attributes are done here
	marketChange.insert(0, marketChange[0] == '-' ? "" : "+");
	marketChange.insert(1, "$");
	marketChangePercent.insert(0, marketChangePercent[0] == '-' ? "" : "+");
	
	return ticker + ": $" + marketPrice + "\n" 
		      + padding + "Change:     (" + marketChange + ", " + marketChangePercent + "%)\n"
		      + padding + "Range:      [$" + marketLow + ", $" + marketHigh + "]\n"
		      + padding + "Close/Open: {$" + marketClose + ", $" + marketOpen + "}"
		      + (!isCrypto ? "\n" + padding + "Bid/Ask:    <$" + bid + ", $" + ask + ">" : "") + "\n";
}

//Executed when "day averages" is included in the query
string Data::getDayAverages()
{	
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";

	string padding(ticker.size() + 2, ' ');
	string marketPrice = formatNumber(getAttribute("\"regularMarketPrice\":"));
	string FDA         = formatNumber(getAttribute("\"fiftyDayAverage\":"));
	string FDAC        = formatNumber(getAttribute("\"fiftyDayAverageChange\":"));
	string FDACP       = formatNumber(getAttribute("\"fiftyDayAverageChangePercent\":"));
	string THDA        = formatNumber(getAttribute("\"twoHundredDayAverage\":"));
	string THDAC       = formatNumber(getAttribute("\"twoHundredDayAverageChange\":"));
	string THDACP      = formatNumber(getAttribute("\"twoHundredDayAverageChangePercent\":"));

	//Any modifications made to the attributes are done here
	FDAC.insert(0, FDAC[0] == '-' ? "" : "+");
	FDAC.insert(1, "$");
	FDACP.insert(0, FDACP[0] == '-' ? "" : "+");
	THDAC.insert(0, THDAC[0] == '-' ? "" : "+");
	THDAC.insert(1, "$");
	THDACP.insert(0, THDACP[0] == '-' ? "" : "+");

	return ticker + ": $" + marketPrice + "\n"
	              + padding + "50 Day:  $" + FDA + " (" + FDAC + ", " + FDACP + "%)\n"
		      + padding + "200 Day: $" + THDA + " (" + THDAC + ", " + THDACP + "%)\n";
}

//Executed when "volumes" is included in the query
string Data::getVolumes()
{
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";
	
	string padding(ticker.size() + 2, ' ');
	string marketPrice        = formatNumber(getAttribute("\"regularMarketPrice\":"));
	string marketVolume       = formatNumber(getAttribute("\"regularMarketVolume\":"));
	string averageDaily3Month = formatNumber(getAttribute("\"averageDailyVolume3Month\":"));
	string averageDaily10Day  = formatNumber(getAttribute("\"averageDailyVolume10Day\":"));

	return ticker + ": $" + marketPrice + "\n"
	              + padding + "Today's Volume:        " + marketVolume + "\n"
		      + padding + "Average Daily 3 Month: " + averageDaily3Month + "\n"
		      + padding + "Average Daily 10 Day:  " + averageDaily10Day + "\n";
}

//Executed when "year" is included in the query
string Data::getYear()
{
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";

	string padding(ticker.size() + 2, ' ');
	string marketPrice           = formatNumber(getAttribute("\"regularMarketPrice\":"));
	string yearLow               = formatNumber(getAttribute("\"fiftyTwoWeekLow\":"));
	string yearHigh              = formatNumber(getAttribute("\"fiftyTwoWeekHigh\":"));
	string yearLowChange         = formatNumber(getAttribute("\"fiftyTwoWeekLowChange\":"));
	string yearLowChangePercent  = formatNumber(getAttribute("\"fiftyTwoWeekLowChangePercent\":"));
	string yearHighChange        = formatNumber(getAttribute("\"fiftyTwoWeekHighChange\":"));
	string yearHighChangePercent = formatNumber(getAttribute("\"fiftyTwoWeekHighChangePercent\":"));

	//Any modifications made to the attributes are done here
	yearLowChange.insert(0, yearLowChange[0] == '-' ? "" : "+");
	yearLowChange.insert(1, "$");
	yearLowChangePercent.insert(0, yearLowChangePercent[0] == '-' ? "" : "+");
	yearHighChange.insert(0, yearHighChange[0] == '-' ? "" : "+");
	yearHighChange.insert(1, "$");
	yearHighChangePercent.insert(0, yearHighChangePercent[0] == '-' ? "" : "+");
	
	return ticker + ": $" + marketPrice + "\n"
		      + padding + "Fifty-Two Week Range:       [$" + yearLow + ", $" + yearHigh + "]\n"
		      + padding + "Fifty-Two Week Low Change:  (" + yearLowChange + ", " + yearLowChangePercent + "%)\n"
		      + padding + "Fifty-Two Week High Change: (" + yearHighChange + ", " + yearHighChangePercent + "%)\n";
}

string Data::getPrice()
{
	if (!isValidTicker)
		return "Error: Invalid Ticker Symbol\n";

	return formatNumber(getAttribute("\"regularMarketPrice\":"));
}
