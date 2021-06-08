#include <iostream>
#include <functional>
#include <thread>
#include <future>
using namespace std;

double giveLoan(future<double>& f)
{
	double loan = f.get();

	return loan;
}

int main()
{
	promise<double> p;
	future<double> f = p.get_future();
	future<double> f2 = async(giveLoan, ref(f));
	
	p.set_value(100);

	cout << f2.get() << endl;

	return 0;
}
