all: ConditionVariable Future Promise SharedFuture PackagedTask Chrono Duration

ConditionVariable: ConditionVariable.cpp
	g++ -g -Wall -o ConditionVariable ConditionVariable.cpp -pthread

Future: Future.cpp
	g++ -g -Wall -o Future Future.cpp -pthread

Promise: Promise.cpp
	g++ -g -Wall -o Promise Promise.cpp -pthread

SharedFuture: SharedFuture.cpp
	g++ -g -Wall -o SharedFuture SharedFuture.cpp -pthread

PackagedTask: PackagedTask.cpp
	g++ -g -Wall -o PackagedTask PackagedTask.cpp -pthread

Chrono: Chrono.cpp
	g++ -g -Wall -o Chrono Chrono.cpp -pthread

Duration: Duration.cpp
	g++ -g -Wall -o Duration Duration.cpp -pthread

TimePoint: TimePoint.cpp
	g++ -g -Wall -o TimePoint TimePoint.cpp -pthread

clean: 
	rm ConditionVariable Future Promise SharedFuture PackagedTask Chrono Duration
