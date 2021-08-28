all: Client Server

Server: Server.cpp Data.cpp FetchData.cpp Data.h Libraries.h
	g++ -g -Wall Server.cpp Data.cpp FetchData.cpp -o Server -pthread -lcurl

runServer: Server
	./Server

Client: Client.cpp Libraries.h
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

Data: Data.h Libraries.h
	g++ -g -Wall Data.cpp -o Data

Login: Login.cpp
	g++ -g -Wall Login.cpp -o Login

clean:
	rm Server Client Data FetchData Login
