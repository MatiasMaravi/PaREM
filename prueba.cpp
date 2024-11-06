#include <mpi.h>
#include <vector>
#include <iostream>

const int N = 2; // Número de filas
const int M = 7; // Número de columnas

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

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Crear una matriz de ejemplo
    int matrix[N][M];
    if(rank == 0) {
        matrix[0][0] = 2; matrix[0][1] = 2; matrix[0][2] = 2;
        matrix[0][3] = 2; matrix[0][4] = 2; matrix[0][5] = 2; matrix[0][6] = 2;
        matrix[1][0] = 1; matrix[1][1] = 1; matrix[1][2] = 1;
        matrix[1][3] = 1; matrix[1][4] = 1; matrix[1][5] = 2; matrix[1][6] = 1;
    } else if(rank == 1) {
        matrix[0][0] = 1; matrix[0][1] = 2; matrix[0][2] = 2;
        matrix[0][3] = 2; matrix[0][4] = 2; matrix[0][5] = 2; matrix[0][6] = 2;
        matrix[1][0] = 0; matrix[1][1] = 0; matrix[1][2] = 1;
        matrix[1][3] = 0; matrix[1][4] = 1; matrix[1][5] = 0; matrix[1][6] = 0;
    } else if(rank == 2) {
        matrix[0][0] = 5; matrix[0][1] = 5; matrix[0][2] = 5;
        matrix[0][3] = 5; matrix[0][4] = 5; matrix[0][5] = 5; matrix[0][6] = 5;
        matrix[1][0] = 0; matrix[1][1] = 0; matrix[1][2] = 1;
        matrix[1][3] = 0; matrix[1][4] = 1; matrix[1][5] = 0; matrix[1][6] = 0;
    } else if(rank == 3) {
        matrix[0][0] = 5; matrix[0][1] = 5; matrix[0][2] = 5;
        matrix[0][3] = 5; matrix[0][4] = 5; matrix[0][5] = 5; matrix[0][6] = 5;
        matrix[1][0] = 0; matrix[1][1] = 0; matrix[1][2] = 0;
        matrix[1][3] = 0; matrix[1][4] = 0; matrix[1][5] = 1; matrix[1][6] = 0;
    }



    // Matriz intermedia y final
    int intermediate_matrix[N][M] = {0};
    int final_matrix[N][M] = {0};

    if (rank == 0 || rank == 1) {
        // Procesos 0 y 1: enviar y recibir entre sí
        if (rank == 0) {
            MPI_Recv(intermediate_matrix, N * M, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(matrix, intermediate_matrix, nullptr, nullptr);

        } else {
            MPI_Send(&matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } else if (rank == 2 || rank == 3) {
        // Procesos 2 y 3: enviar y recibir entre sí
        if (rank == 2) {
            MPI_Recv(intermediate_matrix, N * M, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(matrix,intermediate_matrix, nullptr, nullptr);

        } else {
            MPI_Send(&matrix[0][0], N * M, MPI_INT, 2, 0, MPI_COMM_WORLD);
        }
    }

    // Reducir los resultados de los pares de procesos
    if (rank == 0) {
        MPI_Recv(&final_matrix, N * M, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        reduction_op(&intermediate_matrix, &final_matrix, nullptr, nullptr);
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < M; j++) {
                    std::cout << final_matrix[i][j] << " ";
                }
                std::cout << std::endl;
            }
    } else if (rank == 2) {
        MPI_Send(&intermediate_matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
