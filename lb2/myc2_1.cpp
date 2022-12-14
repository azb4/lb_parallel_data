#include <iostream>
#include <chrono>
#include <omp.h>

using namespace std;

float getfloatnumber() {
	return (float)rand()/RAND_MAX;
}

void multiplicateMatrix(float** a, float** b, float** c1, const int N) {



	int i, j, k;

	omp_set_num_threads(8);

#pragma omp parallel for private(i,j,k) shared(a,b,c1)


		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				for (k = 0; k < N; ++k) {
					c1[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	
}

int main() {
	
	const int N = 4096;

	float **a, **b, **c, **c1;

	a = new float *[N];
	b = new float *[N];
	c = new float *[N];
	c1 = new float* [N];


	srand(time(0));

	for (int i = 0; i < N; i++) {

		a[i] = new float[N];
		b[i] = new float[N];
		c[i] = new float[N];
		c1[i] = new float[N];
	}

	

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			 
			a[i][j] = getfloatnumber();
			b[i][j] = getfloatnumber();
			c[i][j] = 0;
			c1[i][j] = 0;

		}
	cout << "START PARALLEL MULTIPLICATION\n";

	auto start1 = std::chrono::system_clock::now();

	multiplicateMatrix(a, b, c1, N);

	auto end1 = std::chrono::system_clock::now();

	auto division1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();

	cout << division1 << " msec" << endl;

	cout << "START SINGLE MULTIPLICATION\n";



	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < N; i++) {

		for (int j = 0; j < N; j++) {

			for (int k = 0; k < N; k++) {

				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}


	auto end = std::chrono::system_clock::now();

	auto division = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	cout << division << " msec" << endl;

	
	bool flag = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			
			if (c[i][j] != c1[i][j]) {
				flag = 1;
				break;
			}
		}
	}

	if (flag == 1) {

		std::cout << "not equal";

	}
	else 
	{
		std::cout << "equal";
	}
	

}