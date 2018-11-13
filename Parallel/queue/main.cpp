#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

class ThreadSafeQueue
{
	queue<int> q;
	mutex m;

public:

	void Push(int i)
	{
		lock_guard<mutex> lg(m);
		q.push(i);
		cout << "push " << i << endl;
	}

	bool TryPop(int& e)
	{

	}

	bool Pop(int& e)
	{
		lock_guard<mutex> lg(m);
		e = q.front();
		q.pop();
		cout << "pop " << e << endl;
		return e;
	}


	bool IsEmpty()
	{
		lock_guard<mutex> lg(m);
		return q.empty();
	}
};


void ThreadPush(ThreadSafeQueue* q, int iterations)
{
	for (int i = 0; i < iterations; i++)
		q->Push(i);
	q->Push(-1);
}


void ThreadPop(ThreadSafeQueue* q)
{
	while (true) {
		if (!q->IsEmpty()) {
			int e;
			q->Pop(e);
			if (e == -1)
				break;
		}
	}
}


int main()
{
	ThreadSafeQueue q;
	thread pusher(ThreadPush, &q, 10);
	thread poper(ThreadPop, &q);

	pusher.join();
	poper.join();

	system("pause");
	return 0;
}