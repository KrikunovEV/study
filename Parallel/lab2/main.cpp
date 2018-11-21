//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <chrono>
//
//
//using namespace std;
//using namespace chrono;
//
//const int N = 3;
//const int EatingTime = 3000;
//timed_mutex vilki[N];
//mutex text;
//mutex request;
//
//
//void Dinner(int id)
//{
//	int next = (id + 1) == N ? 0 : (id + 1);
//
//	//request.lock();
//
//	bool f = vilki[id].try_lock_for(milliseconds(2 * EatingTime + 1000));
//	if (!f)
//		cout << "error\n";
//
//	f = vilki[next].try_lock_for(milliseconds(2 * EatingTime + 1000));
//	if (!f)
//		cout << "\t\t\terror\n";
//	//request.unlock();
//
//	{
//		lock_guard<mutex> t(text);
//		//cout << "philosopher " << id << " take " << id << "th and " << next << "th vilki" << endl;
//	}
//
//	this_thread::sleep_for(milliseconds(EatingTime));
//
//	vilki[id].unlock();
//	vilki[next].unlock();
//
//	
//	{
//		lock_guard<mutex> t(text);
//		//cout << "philosopher " << id << " have ate. Give away " << id << "th and " << next << "th vilki" << endl;
//	}
//
//}
//
//
//int main()
//{
//	thread* filo = new thread[N];
//
//	for (int i = 0; i < N; i++)
//		filo[i] = thread(Dinner, i);
//
//	for (int i = 0; i < N; i++)
//		filo[i].join();
//
//	delete[] filo;
//
//	cout << "DONE" << endl << endl;
//
//	system("pause");
//	return 0;
//}