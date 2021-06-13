all: ConditionVariable Future Promise SharedFuture PackagedTask Clocks

ConditionVariable:
	g++ -g -o ConditionVariable ConditionVariable.cpp -pthread

Future:
	g++ -g -o Future Future.cpp -pthread

Promise:
	g++ -g -o Promise Promise.cpp -pthread

SharedFuture:
	g++ -g -o SharedFuture SharedFuture.cpp -pthread

PackagedTask:
	g++ -g -o PackagedTask PackagedTask.cpp -pthread

Clocks:
	g++ -g -o Clocks Clocks.cpp

clean:
	rm ConditionVariable Future Promise SharedFuture PackagedTask Clocks
