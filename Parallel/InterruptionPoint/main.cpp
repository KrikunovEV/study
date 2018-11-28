#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

bool flag;

class InterruptedThread
{
private:
	thread t;

public:
	template <typename F>
	InterruptedThread(F func) {
		t = thread(func);
		
	}

	void Interrupt()
	{
		flag = true;
	}

	void join() {
		t.join();
	}

};


void InterruptPoint()
{
	if (flag)
		throw "I was interrupted";
}


void func()
{
	while (true)
	{
		this_thread::sleep_for(milliseconds(500));
		cout << "Im working" << endl;
		try
		{
			InterruptPoint();
		}
		catch (...) {
			break;
		}
	}
}


int main()
{
	InterruptedThread t(func);

	t.Interrupt();

	t.join();

	system("pause");
	return 0;
}
