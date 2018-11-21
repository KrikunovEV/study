#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>


using namespace std;
using namespace chrono;

const int N = 10;
const int EatingTime = 3000;
mutex vilki[N];
mutex text;
bool eating[N];


void Dinner(int id, int* Garry)
{
	bool got[2] = { false, false };

	int next = (id == N - 1) ? 0 : (id + 1);
	
	while (!eating[id]) {

		int last = 0;
		for (int i = 0; i < N; i++)
			last += eating[i];

		if (!got[0] && (*Garry != id || last == N - 1)) {
			got[0] = vilki[id].try_lock();
			if (got[0]) {
				{
					lock_guard<mutex> t(text);
					cout << "philosopher " << id << " take " << id << "th vilky" << endl;
				}
			}
		}

		if (!got[1]) {
			got[1] = vilki[next].try_lock();
			if (got[1]) {
				{
					lock_guard<mutex> t(text);
					cout << "philosopher " << id << " take " << next << "th vilky" << endl;
				}
			}
		}

		
		eating[id] = got[0] && got[1];
	}

	/*if (*Garry == id) {
		do
		{
			(*Garry)--;
			if (*Garry == -1)
				*Garry = N - 1;
		} while (eating[*Garry] && *Garry != id);
		if (*Garry != id)
			vilki[*Garry].unlock();

		lock_guard<mutex> m(text);
		cout << "Garry now is " << *Garry << endl;
	}*/

	this_thread::sleep_for(milliseconds(EatingTime));

	{
		lock_guard<mutex> t(text);
		cout << "philosopher " << id << " have ate. Give away " << id << "th and " << next << "th vilki" << endl;
	}
	vilki[id].unlock();
	vilki[next].unlock();
}


int main()
{
	thread* filo = new thread[N];

	int Garry = N - 1;

	for (int i = 0; i < N; i++)
		filo[i] = thread(Dinner, i, &Garry);

	for (int i = 0; i < N; i++)
		filo[i].join();

	delete[] filo;

	cout << endl << "DONE" << endl << endl;

	system("pause");
	return 0;
}