all: Client Server

Server: Server.cpp Data.h Libraries.h
	g++ -g -Wall Server.cpp Data.cpp -o Server -pthread -lcurl

runServer: Server
	./Server

Client: Client.cpp Libraries.h
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

Data: Data.h Libraries.h
	g++ -g -Wall Data.cpp -o Data

Scraper: Scraper.cpp
	g++ -g -Wall Scraper.cpp -o Scraper -lcurl

clean:
	rm Server Client Data Scraper
