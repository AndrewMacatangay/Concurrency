#include <iostream>
#include <functional>
#include <thread>
#include <future>
using namespace std;

double giveLoan(future<double>& f1)
{
	//f1 waits and finally recieves the value from
	//main() and the promise is fulfilled
	double loan = f1.get();

	//The initial loan was returned to the lender
	//
	return loan;
}

int main()
{
	//A promise is made
	promise<double> p;

	//f1 is able to retrieve a value from the main()
	//function in the future as promised
	future<double> f1 = p.get_future();

	//f2 is able to retrieve a value from the giveLoan()
	//function in the future
	future<double> f2 = async(giveLoan, ref(f1));
	
	//The value promised is sent to the future f1
	p.set_value(100);

	//f2 revieves the return from giveLoan
	cout << f2.get() << endl;

	return 0;
}
