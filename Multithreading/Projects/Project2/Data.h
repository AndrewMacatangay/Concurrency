class Data
{
	private:
	string ticker, JSON, buffer;
	size_t attributeIndex;

	public:
	Data(string ticker, string curlBuffer) : ticker(ticker), JSON(curlBuffer) { }

	string getPrice()
	{
		attributeIndex = JSON.find("regularMarketPrice");
		if (attributeIndex == JSON.npos)
			return "Ticker symbol not found!";

		buffer = JSON.substr(attributeIndex + 20);
		buffer = buffer.substr(0, buffer.find(','));
		buffer[buffer.size() - 2] == '.' && &(buffer += '0');
		return ticker + ": " + buffer;
	}
};
