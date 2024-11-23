#include <iostream>
#include "../automatas/banana_word.h"
#include <mpi.h>

using std::cout;
using std::endl;

const int M = NUM_STATES; // Number of states
const int N = 2; // L and R

string T = "abananabananabaabananabananaababanan";


void reduction_op(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype) {
    int *in = static_cast<int*>(invec);
    int *inout = static_cast<int*>(inoutvec);
    
    int aux_L[M];
    int aux_R[M];
    
    // Realizamos la operación para cada índice x
    for (int x = 0; x < M; ++x) {
        aux_L[x] = inout[in[x]];
        aux_R[x] = in[x + M] + inout[in[x] + M];
    }
    
    for (int x = 0; x < M; ++x) {
        inout[x] = aux_L[x];
        inout[x + M] = aux_R[x];
    }
}

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
    /// BINARY REDUCTION
    
    // Matriz intermedia y final
    int matrix[N][M];
    int intermediate_matrix[N][M] = {0};
    int final_matrix[N][M] = {0};
    for (int i = 0; i < M; i++) {
        matrix[0][i] = L[i];
        matrix[1][i] = R[i];
    }
    if (mpi_rank == 0 || mpi_rank == 1) {
        // Procesos 0 y 1: enviar y recibir entre sí
        if (mpi_rank == 0) {
            MPI_Recv(intermediate_matrix, N * M, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(matrix, intermediate_matrix, nullptr, nullptr);

        } else {
            MPI_Send(&matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } else if (mpi_rank == 2 || mpi_rank == 3) {
        // Procesos 2 y 3: enviar y recibir entre sí
        if (mpi_rank == 2) {
            MPI_Recv(intermediate_matrix, N * M, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(matrix,intermediate_matrix, nullptr, nullptr);

        } else {
            MPI_Send(&matrix[0][0], N * M, MPI_INT, 2, 0, MPI_COMM_WORLD);
        }
    }

    // Reducir los resultados de los pares de procesos
    if (mpi_rank == 0) {
        MPI_Recv(&final_matrix, N * M, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        reduction_op(&intermediate_matrix, &final_matrix, nullptr, nullptr);
        cout << "Cantidad de matcheos: " << final_matrix[1][q0] << endl; 
    } else if (mpi_rank == 2) {
        MPI_Send(&intermediate_matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
