#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <mpi.h>
#define NUM_STATES 9
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
int F = 8; // Final state
int q0 = 0;

unordered_map <char, int> sigma = {
    {'p', 0},
    {'a', 1},
    {'r', 2},
    {'e', 3},
    {'l', 4}
};



int main(int argc, char **argv) {
    int mpi_rank, mpi_size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int j = mpi_rank * (T.size() / mpi_size);
    int end_position = (mpi_rank + 1) * (T.size() / mpi_size) - 1;
    vector<int> L(NUM_STATES);
    vector<int> R(NUM_STATES);
    for (int k = 0; k < NUM_STATES; k++) {
        L[k] = k;
        R[k] = 0;
    }

    while (j <= end_position) {
        for (int i = 0; i < NUM_STATES; i++) {
            L[i] = Tt[L[i]][sigma[T[j]]];
            if (L[i] == F) { // If the state is final
                R[i] = R[i] + 1;
            }
        }
        j = j + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    //Falta comunicar los resultados de los procesos.

    MPI_Finalize();
    return 0;
}
