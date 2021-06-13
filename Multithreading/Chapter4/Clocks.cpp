#include <iostream>
#include <chrono>
using namespace std;

int main()
{
	cout << "System clock:" << endl;
	cout << chrono::system_clock::to_time_t(chrono::system_clock::now()) << endl << endl;

	//cout << "Steady clock:" << endl;
	//cout << chrono::system_clock::to_time_t(chrono::steady_clock::now()) << endl << endl;

	//cout << "High resolution clock:" << endl;
	//cout << chrono::high_resolution_clock::now() << endl << endl;

	return 0;
}
