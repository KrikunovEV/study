#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <omp.h>

using namespace std;

float Rand(float min, float max)
{
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}

float Func(float x, float R)
{
	return sqrt(R*R - x*x);
}


int main()
{
	float R = 1.0f;
	float a = 0.0f, b = R, h = R;
	int N = 1000000, c = 0;

	srand(time(0));

#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		float x = Rand(a, b);
		float y = Rand(0.0f, h);

		if (Func(x, R) >= y)
			#pragma omp atomic
			c++;
		//#pragma omp critical
		//cout << &c << endl;
	}

	float S = (b - a) * h * ((float)c / (float)N);
	cout << "S = " << S << endl;
	cout << "Pi = " << S * 4 / (R*R) << endl;

	system("pause");
	return 0;
}