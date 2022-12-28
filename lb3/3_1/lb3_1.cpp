#include <stdio.h>
#include <random>
#include <iostream>
#include <chrono>
#include <string>

#include "mpi.h"
#include "md5.h"

using namespace std;



string getPassword(const string hash, int step, int r, string allowedChars, int iter) {

    int startIndex = step * iter;
    int endIndex = startIndex + step + r;
    int length = allowedChars.length();

    
    string result;

    for (int i = startIndex; i < endIndex; i++) {

        for (int j = 0; j < length; j++) {

            for (int k = 0; k < length; k++) {

                for (int l = 0; l < length; l++) {

                    result = "";
                    result.push_back((char)allowedChars.at(i));
                    result.push_back((char)allowedChars.at(j));
                    result.push_back((char)allowedChars.at(k));
                    result.push_back((char)allowedChars.at(l));
                    string anotherHash = md5(result);

                    if (hash.compare(anotherHash) == 0) {
                        return result;
                    }
                }
            }
        }
    }

    return "NO";
}

int main() {

    int tag = 0;
    int nProc, rank;
    string allowedChars("1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
    int allowedCharsCount = allowedChars.length();
    int allowedLength = 4;
    string password;

    

    MPI_Init(NULL, NULL);
    MPI_Status status;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &nProc);
    MPI_Comm_rank(comm, &rank);

    char buf[33];
    char pwd[5];

    if (rank == 0) {

        srand(time(NULL));

        for (int i = 0; i < allowedLength; ++i) {
            password += allowedChars.at((int)(rand() % allowedCharsCount));
        }

        const string md5hash = md5(password);
        cout << "Password: " << password << endl;
        cout << "MD5 hash: " << md5hash << endl;

        auto start = chrono::system_clock::now();

        for (int i = 1; i < nProc; i++) {

            string tempMd5hash = md5hash;

            tempMd5hash += to_string(i);

            strcpy_s(buf, tempMd5hash.c_str());

            MPI_Send(buf, (int)tempMd5hash.size(), MPI_CHAR, i, tag, comm);

        }


        MPI_Recv(pwd, allowedLength + 1, MPI_CHAR, MPI_ANY_SOURCE, tag, comm, &status);

        string result = string(pwd);

       

        if (password.compare(result) == 0) {

            cout << "Result: " << result << endl;
            auto end = chrono::system_clock::now();
            auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << time << " msec\n" << endl;

        }
        else {
            cerr << "NO" << endl;
        }

        MPI_Abort(comm, 1);
    }
    else{

        int step;
        if (nProc == 1) 

            step = 1;
        
        else
            step = nProc - 1;

        

        int size;

        int flag = 1;


        MPI_Recv(buf, 33, MPI_CHAR, 0, tag, comm, &status);


        string hash = "";

        for (int i = 0; i < 32; i++) {

            hash += buf[i];

        }


        int iter = atoi(&buf[32]);

        

        int r;

        if (rank == nProc - 1)

            r = (int)allowedCharsCount % nProc;

        else

            r = 0;


        string newpassword = getPassword(hash, (int)step, r, allowedChars, iter - 1);

        if (newpassword.compare("No") != 0) {
 
            strcpy_s(pwd, newpassword.c_str());

            pwd[4] = 0;

            MPI_Send(pwd, allowedLength + 1, MPI_CHAR, 0, tag, comm);

        }
    }

    MPI_Finalize();
}
