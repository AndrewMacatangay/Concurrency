all: ConditionVariable Future Promise SharedFuture PackagedTask Duration TimePoint

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

Duration: Duration.cpp
	g++ -g -Wall -o Duration Duration.cpp -pthread

TimePoint: TimePoint.cpp
	g++ -g -Wall -o TimePoint TimePoint.cpp -pthread

clean: 
	rm ConditionVariable Future Promise SharedFuture PackagedTask Duration TimePoint
