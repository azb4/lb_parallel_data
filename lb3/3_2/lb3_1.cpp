#include <stdio.h>
#include <random>
#include <iostream>
#include <chrono>
#include <string>

#include "mpi.h"

using namespace std;


float getfloatnumber() {

	return (float)rand() / RAND_MAX;

}



int main() {

	const int N = 4096;
	int flag = 0;
	int tag = 0;


	

	int average = 0, average1 = 0;

	int nProc, rank;

	MPI_Init(NULL, NULL);
	MPI_Status status;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &nProc);
	MPI_Comm_rank(comm, &rank);

	int step = N / (nProc-1);

	auto a = new float[N][N];
	auto b = new float[N][N];
	auto c = new float[N][N];
	auto c1 = new float[N][N];
	auto c2 = new float[N][N];

	int laststep = (nProc-1) * step;

	int ch = 0;

	if (rank == 0) {

			srand(time(0));


			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++) {

					a[i][j] = getfloatnumber();
					b[i][j] = getfloatnumber();
					c[i][j] = 0;
					c1[i][j] = 0;

				}

			
			cout << "START PARALLEL MULTIPLICATION\n";

			auto start1 = chrono::system_clock::now();


			for (int i = 1; i < nProc; i++) {
				
				MPI_Send(&(a[0][0]), N * N, MPI_FLOAT, i, tag, comm);
				MPI_Send(&(b[0][0]), N * N, MPI_FLOAT, i, tag + 1, comm);
				MPI_Send(&(c1[0][0]), N * N, MPI_FLOAT, i, tag + 2, comm);


			}

			for (int i = 1; i < nProc; i++) {

				MPI_Recv(&(c1[0][0]), N * N, MPI_FLOAT, i, tag, comm, &status);
				for (int j = 0; j < N; j++) {
					if (c1[j][0] != 0) {
						for (int n = 0; n < N; n++) {
							c2[j][n] = c1[j][n];
						}
					}
					/*cout << "\n";
					for (int k = 0; k < N; k++) {
						cout << c2[j][k] << " ";
					}*/
					
				
				}
				
			}

			auto end1 = chrono::system_clock::now();

			auto division1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();

			cout << division1 << " msec" << endl;

			/*MPI_Recv(&(c1[0][0]), N * N, MPI_FLOAT, MPI_ANY_SOURCE, tag, comm, &status);
			*/

			cout << "START SINGLE MULTIPLICATION\n";

			auto start = chrono::system_clock::now();

			for (int i = 0; i < N; i++) {

				for (int j = 0; j < N; j++) {

					for (int k = 0; k < N; k++) {

						c[i][j] += a[i][k] * b[k][j];
					}
				}
			}


			auto end = chrono::system_clock::now();

			auto division = chrono::duration_cast<chrono::milliseconds>(end - start).count();

			cout << division << " msec" << endl;

			



			bool flag = 0;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {

					if (c[i][j] - c2[i][j] != 0) {

						cout << c[i][j] << " " << c2[i][j] << " " << i << " " << j << "\n";
						flag = 1;
						break;
					}
				}
			}

			if (flag == 1) {

				std::cout << "not equal\n";

			}
			else
			{
				std::cout << "equal\n";
			}

			//		int i, j, k;

			//		if (laststep == last)
			//			last = N;

			//		for (int i = first; i < last; ++i) {
			//			for (int j = 0; j < N; j++) {
			//				for (int k = 0; k < N; k++) {
			//					c1[i][j] += a[i][k] * b[k][j];
			//				}
			//			}
			//		}

			//	}

			//	int laststep = step * NUM_THREADS;


			//	for (int i = 0; i < NUM_THREADS; i++) {
			//		threads[i] = thread(multiplicateMatrix, N, i * step, (i + 1) * step, a, b, c1, laststep);
			//	}

			//	for (int i = 0; i < NUM_THREADS; i++) {
			//		threads[i].join();
			//	}

				

				

	

		
		
	}

	else {

		MPI_Recv(&(a[0][0]), N * N, MPI_FLOAT, 0, tag, comm, &status);
		/*cout << a[1][1] << " a ";*/
		MPI_Recv(&(b[0][0]), N * N, MPI_FLOAT, 0, tag + 1, comm, &status);
		/*cout << b[1][1] << " b ";*/
		MPI_Recv(&(c1[0][0]), N * N, MPI_FLOAT, 0, tag + 2, comm, &status);




		int first = (rank - 1) * step;


		int last = (rank)*step;

		int i, j, k;

		if (laststep == last) {
		last = N;
	}



			for (int i = first; i < last; ++i) {

				for (int j = 0; j < N; j++) {

					for (int k = 0; k < N; k++) {

						c1[i][j] += a[i][k] * b[k][j];

						

					}
					/*cout << c1[i][0] << " c " << i << i <<"\n";*/
				}
			}

			MPI_Send(&(c1[0][0]), N * N, MPI_FLOAT, 0, tag, comm);
			
			

		}

			
		MPI_Finalize();

		
}
