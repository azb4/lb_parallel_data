#include <iostream>
#include <thread>
#include <chrono>

#define NUM_THREADS 8

using namespace std;

void sumFunction(int pos, unsigned long long first, unsigned long long last, unsigned long long* sumArray, unsigned long long laststep, unsigned long long N) {

    unsigned long long sum = 0;

    if (last == laststep)
        last = N-1;
    

    for (long long i = first; i <= last; ++i) {
        sum += i;
    }
    sumArray[pos] = sum;
}

int main() {

    thread threads[NUM_THREADS];
    unsigned long long sumArray[NUM_THREADS] = { 0 };
    unsigned long long sum1 = 0, sum2 = 0, N = 10000000000; //10
    unsigned long long step = N / NUM_THREADS;
    int av = 0, av1 = 0;


    for (int l = 0; l < 10; l++) {
        sum1 = 0, sum2 = 0;
        cout << l <<"\n" << "MultiStart" << endl;

        auto start = chrono::system_clock::now();

        unsigned long long laststep = step * NUM_THREADS;


        for (int i = 0; i < NUM_THREADS; i++) {

            threads[i] = thread(sumFunction, i, i * step + 1, (i + 1) * step, sumArray, laststep, N);
        }

        for (int i = 0; i < NUM_THREADS; i++) {

            threads[i].join();
        }

        for (int i = 0; i < NUM_THREADS; i++) {

            sum1 += sumArray[i];
        }

        auto end = chrono::system_clock::now();
        auto time1 = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "MultiEnd" << std::endl;
        cout << "Sum 1 = " << sum1 << " with time: " << time1 << " msec" << endl;

        cout << "SingleStart" << endl;
        start = chrono::system_clock::now();

        for (long long i = 0; i < N; i++) {
            sum2 += i;
        }

        end = chrono::system_clock::now();

        auto time2 = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "SingleThread end" << endl;
        cout << "Sum 2 = " << sum2 << " with time: " << time2 << " msec" << endl;

        av1 += time2;
        av += time1;
        if (sum1 == sum2)
            cout << "Sums are equal\n";

        else
            cout << "Sums are not equal\n";
    }
    cout << "Avarage time in Single - " << av1 / 10 << " msec, in Multi - " << av / 10 << " msec";
}