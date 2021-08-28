all: Client Server

Server: Server.cpp Data.cpp FetchData.cpp Data.h Account.h Libraries.h
	g++ -g -Wall Server.cpp FetchData.cpp Data.cpp Account.cpp -o Server -pthread -lcurl

runServer: Server
	./Server

Client: Client.cpp Libraries.h
	g++ -g -Wall Client.cpp -o Client

runClient: Client
	./Client

Data: Data.h Libraries.h
	g++ -g -Wall Data.cpp -o Data

LoginTest: LoginTest.cpp
	g++ -g -Wall LoginTest.cpp -o LoginTest

clean:
	rm Server Client Data LoginTest
