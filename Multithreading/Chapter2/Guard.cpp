//Program to demonstate wrapper classes to guard
//threads against not joining

#include<iostream>
#include<thread>
using namespace std;

class thread_guard
{
	private:
		thread t;
	public:
		//Guard constructor which moves the ownership
		//of t from main to this->t
		thread_guard(thread& pass)
		{
			t = move(pass);
			cout << "Thread Guard Created!" << endl;
		}
		
		//Since this->t has ownership of the original
		//thread from main, the class will be in charge
		//of calling .join() when the destructor is called
		~thread_guard()
		{
			if (t.joinable())
				t.join();
			cout << "Thread Guard Destroyed!" << endl;
		}
};

void func1()
{
	cout << "Inside func1!" << endl;
}

int main()
{
	//Create new thread and execute function
	thread t(func1);

	//Pass thread to instance of thread guard
	thread_guard tg(t);

	cout << "Inside main!" << endl;
	return 0;
}
