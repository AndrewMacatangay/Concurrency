all: Client Server

Server: Server.cpp
	g++ -g -Wall Server.cpp -o Server -pthread -lcurl

runServer: Server
	./Server

Client: Client.cpp
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

Scraper: Scraper.cpp
	g++ -g -Wall Scraper.cpp -o Scraper -lcurl

clean:
	rm Server Client Scraper
