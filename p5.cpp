#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <cmath>
#include <omp.h>
#define NUM_THREADS 4
using namespace std;

typedef vector<vector<int>> TransitionTable;
typedef set<int> FinalStates;
typedef set<int> States;

TransitionTable Tt = {
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
string T = "parallelparallelparallelparallelparallla";
FinalStates F = {8};
int q0 = 0;
States Q = {0, 1, 2, 3, 4, 5, 6, 7, 8};
unordered_map <char, int> sigma = {
    {'p', 0},
    {'a', 1},
    {'r', 2},
    {'e', 3},
    {'l', 4}
};

// TransitionTable Tt = {
//     {0, 1, 0},
//     {2, 1, 0},
//     {0, 1, 3},
//     {4, 1, 0},
//     {0, 1, 5},
//     {6, 1, 0},
//     {0, 1, 0}
// };

// FinalStates F = {6};

// int q0 = 0;
// States Q = {0, 1, 2, 3, 4, 5, 6};

// string T = "abananabananabaabananabananaababanan";

// unordered_map <char, int> sigma = {
//     {'a', 0},
//     {'b', 1},
//     {'n', 2}
// };


int main(int argc, char **argv) {
    
    int n = T.size();
    omp_set_num_threads(NUM_THREADS);
    vector<vector<int>> L(NUM_THREADS,vector<int>(Q.size()));
    vector<vector<int>> R(NUM_THREADS,vector<int>(Q.size(), 0));
    #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        int size = omp_get_num_threads();
        int j = rank * (n / size);
        int end_position = (rank + 1) * (n/size) - 1;
        for (int k = 0; k < Q.size(); k++) {
            L[rank][k] = k;
        }
        while (j <= end_position) {
            for (int k = 0; k < Q.size(); k++) {
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
                for(int x = 0; x < Q.size(); x++){
                    R[rank][x] += R[rank + (int)pow(2,m-1)][L[rank][x]];
                    L[rank][x] = L[rank + (int)pow(2,m-1)][L[rank][x]];
                }
            }
        }
    }
    cout << "Cantidad de veces que la cadena es aceptada: " << R[0][q0] << endl;
    

    return 0;
}
