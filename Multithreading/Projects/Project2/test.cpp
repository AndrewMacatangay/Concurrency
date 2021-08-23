#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
	fstream f;
	f.open(".//UserData//test.csv", fstream::out);
	f << "Hello World" << endl;
	f.close();
	return 0;
}
