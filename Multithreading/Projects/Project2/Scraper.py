import bs4
import requests
from bs4 import BeautifulSoup
def parsePrice(URL):
	r = requests.get(URL)
	soup = bs4.BeautifulSoup(r.text, 'html5lib')
	price = soup.find('div', {'class': 'D(ib) Mend(20px)'}).find('span').text
	return price

ticker = input("Enter ticker symbol: ")
URL = 'https://finance.yahoo.com/quote/' + ticker.upper();
while True:
	print(parsePrice(URL))
#sleep
