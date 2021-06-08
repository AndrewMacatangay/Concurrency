all: ConditionVariable Future Promise

ConditionVariable:
	g++ -g -o ConditionVariable ConditionVariable.cpp -pthread

Future:
	g++ -g -o Future Future.cpp -pthread

Promise:
	g++ -g -o Promise Promise.cpp -pthread

clean:
	rm ConditionVariable Future Promise
