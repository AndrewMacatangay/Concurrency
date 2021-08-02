all: Client Server

Server: Server.cpp
	g++ -g -Wall Server.cpp -o Server

runServer: Server
	./Server

Client: Client.cpp
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

clean:
	rm Server Client
