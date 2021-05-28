#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
//Protecting shared data
//Protect from passing functions/references/pointers

void badFunc(vector<string>& formula)
{
	formula.clear();
}

class Company
{
	private:
		int companyBalance;
		vector<string> secretFormula;
		mutex m;

		vector<string> getFormula()
		{
			lock_guard<mutex> l(m);
			return secretFormula;
		}
		
	public:
		Company()
		{
			companyBalance = 1000000000;
			secretFormula = {"sesame seed buns", "sea cheese", "sea lettuce",
					 "sea tomatoes", "pickles", "ketchup", "mayonnaise",
					 "sea onions", "secret sauce", "chum"};
			
			cout << "Krabby Patty Recipe: " << endl;
			for (string x : getFormula())
				cout << x << endl;
			cout << endl;
		}

		vector<string>& getFormulaRef()
		{
			lock_guard<mutex> l(m);
			return secretFormula;
		}

		vector<string>* getFormulaVal()
		{
			lock_guard<mutex> l(m);
			return &secretFormula;
		}

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
	Company KrustyKrab;	

	vector<string>& stolenFormula = KrustyKrab.getFormulaRef();
	stolenFormula.erase(stolenFormula.begin() + 4);
	cout << "Krabby Patty Recipe: " << endl;
	for (string x : stolenFormula)
		cout << x << endl;
	cout << endl;
	
	KrustyKrab.getFormulaVal()->insert(stolenFormula.begin() + 4, "pickles");
	cout << "Krabby Patty Recipe: " << endl;
	for (string x : *(KrustyKrab.getFormulaVal()))
		cout << x << endl;
	cout << endl;
	
	KrustyKrab.runFunc(badFunc);
	
	return 0;
}
