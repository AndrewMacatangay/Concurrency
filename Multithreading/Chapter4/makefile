all: ConditionVariable Future Promise SharedFuture PackagedTask Clocks

ConditionVariable: ConditionVariable.cpp
	g++ -g -o ConditionVariable ConditionVariable.cpp -pthread

Future: Future.cpp
	g++ -g -o Future Future.cpp -pthread

Promise: Promise.cpp
	g++ -g -o Promise Promise.cpp -pthread

SharedFuture: SharedFuture.cpp
	g++ -g -o SharedFuture SharedFuture.cpp -pthread

PackagedTask: PackagedTask.cpp
	g++ -g -o PackagedTask PackagedTask.cpp -pthread

Clocks: Clocks.cpp
	g++ -g -o Clocks Clocks.cpp -pthread

clean: 
	rm ConditionVariable Future Promise SharedFuture PackagedTask Clocks
