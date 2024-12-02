#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <omp.h>
#define NUM_STATES 9
#define NUM_THREADS 4
using namespace std;


vector<vector<int>> Tt = {
    {1, 0, 0, 0, 0},
    {1, 2, 0, 0, 0},
    {1, 0, 3, 0, 0},
    {1, 4, 0, 0, 0},
    {1, 0, 0, 0, 5},
    {1, 0, 0, 0, 6},
    {1, 0, 0, 7, 0},
    {1, 0, 0, 0, 8},
    {1, 0, 0, 0, 0}
};
string T = "parallel";
set<int> F = {8};
int q0 = 0;

unordered_map <char, int> sigma = {
    {'p', 0},
    {'a', 1},
    {'r', 2},
    {'e', 3},
    {'l', 4}
};



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
        if(rank == 0){
            int L_temp = q0;
            int R_temp = 0;
            for(int k = 0; k < size; k++){
                R_temp += R[k][L_temp];
                L_temp = L[k][L_temp];
            }
            R[0][q0] = R_temp;
            L[0][q0] = L_temp;
        }
    }
    double end_time = omp_get_wtime();
    cout << "Cantidad de veces que la cadena es aceptada: " << R[0][q0] << endl;

    return 0;
}
