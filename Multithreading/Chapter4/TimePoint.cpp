//The purpose of this program is to demonstrate the use of timepoints.
//It sets a time point 3 seconds into the future and the user must input
//the alphabet correctly before time runs out.

#include<iostream>
#include<string>
#include<chrono>
#include<future>
#include<mutex>
#include<condition_variable>
using namespace std;

mutex m;
bool done = 0;
condition_variable cv;

bool speedTest()
{
	using namespace chrono;
	steady_clock::time_point tp = steady_clock::now() + seconds(3);
	
	unique_lock<mutex> l(m);
	while(!done)
		if (cv.wait_until(l, tp) == cv_status::timeout)
			break;

	return done;
}

int main()
{
	string alphabet;
	future<bool> f = async(launch::async, speedTest);
	
	cout << "Type out the alphabet as fast as you can:" << endl;
	cin >> alphabet;

	if (alphabet == "abcdefghijklmnopqrstuvwxyz")
		done = 1;
	else
		done = 0;

	cout << (f.get() ? "Success!" : "Failed...") << endl;
	return 0;
}
