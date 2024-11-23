#include <iostream>
#include "../automatas/banana_word.h"
#include <cmath>
#include <omp.h>
#define NUM_THREADS 4
using namespace std;


string T = "abananabananabaabananabananaababanan";

int main(int argc, char **argv) {
    
    int n = T.size();
    omp_set_num_threads(NUM_THREADS);
    vector<vector<int>> L(NUM_THREADS,vector<int>(NUM_STATES));
    vector<vector<int>> R(NUM_THREADS,vector<int>(NUM_STATES, 0));
    #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        int size = omp_get_num_threads();
        int j = rank * (n / size);
        int end_position = (rank + 1) * (n/size) - 1;
        for (int k = 0; k < NUM_STATES; k++) {
            L[rank][k] = k;
        }
        while (j <= end_position) {
            for (int k = 0; k < NUM_STATES; k++) {
                L[rank][k] = Tt[L[rank][k]][sigma[T[j]]];
                if (F.find(L[rank][k]) != F.end()) {
                    R[rank][k]++;
                }
            }
            j++;
        }
        #pragma omp barrier
        for(int m = 1; m <= ceil(log2(size));m++){
            if(rank % (int)pow(2,m) == 0 && (rank + (int)pow(2,m-1)) < size){
                for(int x = 0; x < NUM_STATES; x++){
                    R[rank][x] += R[rank + (int)pow(2,m-1)][L[rank][x]];
                    L[rank][x] = L[rank + (int)pow(2,m-1)][L[rank][x]];
                }
            }
        }
    }
    cout << "Cantidad de veces que la cadena es aceptada: " << R[0][q0] << endl;
    

    return 0;
}
