#include <iostream>
#include "../automatas/banana_word.h"
#include <mpi.h>
using std::cout;
using std::endl;

string T = "abananabananabaabananabananaababanan";

int main(int argc, char **argv) {
    int mpi_rank, mpi_size;
    int n = T.size();
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int j = mpi_rank * (n / mpi_size);
    int end_position = (mpi_rank + 1) * (n / mpi_size) - 1;
    vector<int> L(NUM_STATES);
    vector<int> R(NUM_STATES, 0);
    for (int k = 0; k < NUM_STATES; k++) {
        L[k] = k;
    }

    while (j <= end_position) {
        for (int i = 0; i < NUM_STATES; i++) {
            L[i] = Tt[L[i]][sigma[T[j]]];
            if (F.find(L[i]) != F.end()) {
                R[i]++;
            }
        }
        j++;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    int R_temp, L_temp;

    if (mpi_rank == 0) {
        MPI_Send(&R[q0], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&L[q0], 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    }
    else if (mpi_rank > 0 && mpi_rank < mpi_size - 1) {
        MPI_Recv(&R_temp, 1, MPI_INT, mpi_rank - 1, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(&L_temp, 1, MPI_INT, mpi_rank - 1, 1, MPI_COMM_WORLD, &stat);

        R[L_temp] += R_temp;
        MPI_Send(&R[L_temp], 1, MPI_INT, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&L[L_temp], 1, MPI_INT, mpi_rank + 1, 1, MPI_COMM_WORLD);

    }
    else if (mpi_rank == mpi_size - 1) {
        MPI_Recv(&R_temp, 1, MPI_INT, mpi_size - 2, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(&L_temp, 1, MPI_INT, mpi_size - 2, 1, MPI_COMM_WORLD, &stat);
        R[L_temp] += R_temp;
        MPI_Send(&R[L_temp], 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&L[L_temp], 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    if (mpi_rank == 0) {
        MPI_Recv(&R_temp, 1, MPI_INT, mpi_size - 1, 2, MPI_COMM_WORLD, &stat);
        MPI_Recv(&L_temp, 1, MPI_INT, mpi_size - 1, 3, MPI_COMM_WORLD, &stat);
        cout << "Cantidad de veces que la cadena es aceptada: " << R_temp << endl;
    }

    MPI_Finalize();
    return 0;
}
