#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

#define NUM_THREADS 8

void sumFunction(int N, unsigned long long first, unsigned long long last, float** matrixA, float** matrixB, float** productMatrixParallel) {
    for (unsigned long long i = first; i < last; ++i) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                productMatrixParallel[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

int main() {

    std::thread threads[NUM_THREADS];
    const int N = 4096;
    unsigned long long step = N / NUM_THREADS;

    float** matrixA, ** matrixB, ** productMatrixParallel, ** productMatrixSerial;
    matrixA = new float* [N];
    matrixB = new float* [N];
    productMatrixParallel = new float* [N];
    productMatrixSerial = new float* [N];
    srand(time(nullptr));

    for (int i = 0; i < N; i++) {
        matrixA[i] = new float[N];
        matrixB[i] = new float[N];
        productMatrixParallel[i] = new float[N];
        productMatrixSerial[i] = new float[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrixA[i][j] = 1 + rand() % 100;
            matrixB[i][j] = 1 + rand() % 100;
        }
    }

    std::cout << "Start multithread calculation..." << std::endl;
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(sumFunction, N, i * step, (i + 1) * step, matrixA, matrixB, productMatrixParallel);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    auto end = std::chrono::system_clock::now();
    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "End multithread: " << time1 << " msec" << std::endl;


    std::cout << "Start single thread calculation ..." << std::endl;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                productMatrixSerial[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    end = std::chrono::system_clock::now();
    auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "End single thread calculation with time: " << time2 << " msec" << std::endl;

    std::cout << "Matrix check:" << std::endl;
    bool flag = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (productMatrixSerial[i][j] - productMatrixParallel[i][j] != 0) {
                flag = 1;
                break;
            }
        }
    }


    if (flag == 1) {
        std::cout << "not equal" << " ";
    }
    else {
        std::cout << "equal" << " ";
    }
} 