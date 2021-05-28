//Since threads can share data, we need to protect them from
//changing the data when it is being mutated or changed. We
//need to do this by not allowing a data member to be changed
//while another thread is also trying to commit a change. We
//should refrain from passing shared data by reference, address,
//or passing in malicious functions.

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
using namespace std;

//By allowing access to our formula with a malicious function
//we are able to bypass the mutex and change the class' member
//functions from within the class
void badFunc(vector<string>& formula)
{
	formula.clear();
}

class Company
{
	private:
		vector<string> secretFormula;
		mutex m;

		//In order to get the formula, it should only
		//be accessible from within the class
		vector<string> getFormula()
		{
			lock_guard<mutex> l(m);
			return secretFormula;
		}
		
	public:
		Company()
		{
			secretFormula = {"sesame seed buns", "sea cheese", "sea lettuce",
					 "sea tomatoes", "pickles", "ketchup", "mayonnaise",
					 "sea onions", "secret sauce", "chum"};
			
			cout << "Krabby Patty Recipe: " << endl;
			for (string x : getFormula())
				cout << x << endl;
			cout << endl;
		}

		//By returning a reference, we will be able to change
		//the formula from outside of the class, and would be
		//able to bypass the mutex
		vector<string>& getFormulaRef()
		{
			lock_guard<mutex> l(m);
			return secretFormula;
		}

		//By returning an address, we will be able to change
		//the formula from outside of the class, and would be
		//able to bypass the mutex
		vector<string>* getFormulaVal()
		{
			lock_guard<mutex> l(m);
			return &secretFormula;
		}

		//By passing a function to a member function, we will
		//be able to change the fomula via a malicious function
		void runFunc(void func(vector<string>&))
		{
			func(secretFormula);

			cout << "Krabby Patty Recipe: " << endl;
			for (string x : getFormula())
				cout << x << endl;
			cout << endl;
		}
};

//An example of what not to do
int main()
{
	//Creates a new Company instance that contains a secret formula
	Company KrustyKrab;	

	//Plankton has stole the formula and removed the pickles
	vector<string>& stolenFormula = KrustyKrab.getFormulaRef();
	stolenFormula.erase(stolenFormula.begin() + 4);
	cout << "Krabby Patty Recipe: " << endl;
	for (string x : stolenFormula)
		cout << x << endl;
	cout << endl;
	
	//Spongebob revises the formula at home to add the pickles
	KrustyKrab.getFormulaVal()->insert(stolenFormula.begin() + 4, "pickles");
	cout << "Krabby Patty Recipe: " << endl;
	for (string x : *(KrustyKrab.getFormulaVal()))
		cout << x << endl;
	cout << endl;
	
	//Patrick goes to the Krusty Krab and erases the formula
	KrustyKrab.runFunc(badFunc);
	
	return 0;
}
