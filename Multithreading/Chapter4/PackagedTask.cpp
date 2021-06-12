#include<iostream>
#include<deque>
#include<thread>
#include<mutex>
#include<future>
#include<chrono>
using namespace std;

int task(int x)
{
	cout << "The task has been run!" << endl;
	return x;
}

//You want it so that certain threads will run certain
//tasks. Move the tasks into a thread.

//This class will manage all packaged_tasks and futures
//necessary for running the threads that will be created
//in main()
class TaskQueueWrapper
{
	private:
		deque<packaged_task<int(int)>> tasks;
		deque<future<int>> futures;
		int size = 0;
		mutex m;
		condition_variable cv;
	public:
		//Push the tasks onto a queue
		void push_back(packaged_task<int(int)> pt)
		{
			tasks.push_back(move(pt));
			futures.push_back(tasks[size++].get_future());
			cv.notify_one();
		}

		//Returns a packaged_task that can be used in main when
		//creating the threads of execution
		packaged_task<int(int)> pop_front()
		{
			unique_lock<mutex> l(m);
			cv.wait(l, [this]{ return !tasks.empty(); });
			packaged_task<int(int)> task = move(tasks.front());
			tasks.pop_front();
			size--;

			return task;
		}

		void launchThread(int value)
		{
			//No lock needed here since there is a lock within
			//the TaskQueueWrapper::pop_front() function. A
			//deadlock would occur otherwise
			thread(move(pop_front()), value).detach();
		}

		int get()
		{
			lock_guard<mutex> l(m);
			int value = futures.front().get();
			futures.pop_front();

			return value;
		}
};

int main()
{
	//This is a simplified example of a single packaged_task
	packaged_task<int(int)> pt(task);
	future<int> f = pt.get_future();
	
	//If you don't want to associate the packaged_task with
	//a thread, you can execute the function like this:
	//pt(5);

	cout << "Simple example:" << endl;
	thread(move(pt), 0).detach();

	cout << f.get() << endl << endl;
	

	//The rest of the example will use a wrapper class that
	//handles the packaged_tasks and futures
	TaskQueueWrapper tasks;

	//Queue up the tasks
	for(int x = 1; x <= 5; x++)
		tasks.push_back(packaged_task<int(int)>(task));

	cout << "Wrapper class example:" << endl;

	//Execute the function in different threads
	for(int x = 1; x <= 5; x++)
		tasks.launchThread(x);

	//Not an elegant way to block the main thread, but
	//used just to achieve a clean output
	this_thread::sleep_for(chrono::milliseconds(50));

	//Get the values returned from the function which are
	//stored in the futures
	for(int x = 1; x <= 5; x++)
		cout << tasks.get() << endl;
	
	return 0;
}
