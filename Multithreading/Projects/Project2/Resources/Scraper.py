import bs4
import requests
from bs4 import BeautifulSoup

def parsePrice(ticker):
	URL = 'https://finance.yahoo.com/quote/' + ticker.upper();
	r = requests.get(URL)
	soup = bs4.BeautifulSoup(r.text, 'html5lib')
	price = soup.find('div', {'class': 'D(ib) Mend(20px)'}).find('span').text
	
	print(price)

ticker = input("Enter ticker symbol: ")
while True:
	parsePrice(ticker)
