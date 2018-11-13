#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m1, m2;

int s = 0;

void timer()
{
	//...
	lock_guard<mutex> lg(m1);
	s / 0;
	//...
}
int main()
{
	thread t(timer);

	//.....
	{
		lock_guard<mutex> lg(m1);
	
		m2.lock();
		s++;

	}
	//.....

	t.join();
	system("pause");
	return 0;
}