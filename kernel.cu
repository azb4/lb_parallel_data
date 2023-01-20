#include <iostream>
#include <cuda_runtime.h>

#include <stdio.h>

#define BLOCK_SIZE 32 
#define N 32

float getfloatnumber() {

	return (float)rand() / RAND_MAX;

}
__global__ void matMult(float* a, float* b, int n, float* c)

{

	int ROW = blockIdx.y * blockDim.y + threadIdx.y;

	int COL = blockIdx.x * blockDim.x + threadIdx.x;

	float tmpSum = 0;

	if (ROW < n && COL < n) {
		for (int i = 0; i < n; i++) {
			tmpSum += a[ROW * n + i] * b[i * n + COL];
		}
		c[ROW * n + COL] = tmpSum;
	}
	
}

int main(int argc, char* argv[])
{
	int numBytes = N * N * sizeof(float);

	// выделение памяти на хосте

	float* a = new float[N * N];
	float* b = new float[N * N];
	float* c = new float[N * N];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int k = N * i + j;
			a[k] = (float)k;
			b[k] = (float)k;
			c[k] = (float)k;
		}
	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			int k = N * i + j;
			std::cout << a[k] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "====================\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			int k = N * i + j;
			std::cout << b[k] << " ";
		}
		std::cout << "\n";
	}*/

	// выделение памяти на девайсе

	float* adev = NULL;
	float* bdev = NULL;
	float* cdev = NULL;

	cudaMalloc((void**)&adev, numBytes);
	cudaMalloc((void**)&bdev, numBytes);
	cudaMalloc((void**)&cdev, numBytes);


	//dim3 blocks;
	//dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
	//if (N < threads.x || N < threads.y) {
	//	dim3 blocks(1, 1);
	//}
	//else {
	//	dim3 blocks(float(N) / float(threads.x), float(N / threads.y));
	//}


	cudaEvent_t start, stop;
	float gpuTime = 0.0f;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);



	cudaEventRecord(start, 0);
	cudaMemcpy(adev, a, numBytes, cudaMemcpyHostToDevice);
	cudaMemcpy(bdev, b, numBytes, cudaMemcpyHostToDevice);

	/*matMult <<< blocks, threads >>> (adev, bdev, N, cdev);*/

	dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 blocksPerGrid(1, 1);

	if (BLOCK_SIZE * BLOCK_SIZE > 1024) {

		threadsPerBlock.x = 1024;
		threadsPerBlock.y = 1024;

		blocksPerGrid.x = ceil(float(N) / float(threadsPerBlock.x));
		blocksPerGrid.y = ceil(float(N) / float(threadsPerBlock.y));

	}

	matMult << < blocksPerGrid, threadsPerBlock >> > (adev, bdev, N, cdev);

	cudaMemcpy(c, cdev, numBytes, cudaMemcpyDeviceToHost);

	/*std::cout << "====================\n";

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			int k = N * i + j;
			std::cout << c[k] << " ";
		}
		std::cout << "\n";
	}*/
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&gpuTime, start, stop);

	std::cout<< c[N*N-1] << "\n";

	printf("time spent executing by the GPU: %.2f millseconds\n", gpuTime);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(adev);
	cudaFree(bdev);
	cudaFree(cdev);

	delete a;
	delete b;
	delete c;

	return 0;

}