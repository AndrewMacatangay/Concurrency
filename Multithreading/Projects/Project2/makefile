all: Client Server

Server: Server.cpp
	g++ -g -Wall Server.cpp -o Server -pthread

runServer: Server
	./Server

Client: Client.cpp
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

Scrape: Scrape.cpp
	g++ -g -Wall Scrape.cpp -o Scrape -lcurl

clean:
	rm Server Client Scrape
