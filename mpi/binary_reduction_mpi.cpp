#include <iostream>
#include "../automatas/banana_word.h"
#include <mpi.h>

using std::cout;
using std::endl;

const int M = NUM_STATES; // Number of states
const int N = 2; // L and R

string T = 
    "En un pequeño pueblo rodeado de verdes montañas y ríos cristalinos, "
    "existía un mercado famoso por su abundancia de frutas exóticas. Entre todas ellas, "
    "la estrella indiscutible era la banana. La gente venía de lugares lejanos solo para probar "
    "las bananas de este mercado, conocidas por su dulzura y textura perfecta. Pero lo que pocos "
    "sabían era que estas bananas no eran comunes; estaban relacionadas con una leyenda local que "
    "hablaba de un misterioso espíritu frutal.\n\n"

    "Una mañana, Ana, una joven del pueblo, decidió investigar por qué las bananas de su mercado "
    "eran tan especiales. Para Ana, las bananas no solo eran deliciosas, sino también misteriosas. "
    "Había algo mágico en su color amarillo brillante, en cómo el aroma dulce invadía el aire y en "
    "cómo la piel se deslizaba tan fácilmente para revelar la fruta perfecta. “¿De dónde vienen "
    "estas bananas?” se preguntaba.\n\n"

    "Ana comenzó su búsqueda hablando con Don Ernesto, el anciano dueño de la plantación de bananas "
    "más grande de la región. Él le contó que sus bananas crecían en un lugar especial al pie de una "
    "cascada, donde el suelo era rico en minerales y la brisa estaba impregnada de energía ancestral. "
    "Según Don Ernesto, el secreto de sus bananas era el agua de esa cascada, la cual, según la leyenda, "
    "había sido bendecida por el mismo espíritu frutal. “Pero eso es solo un cuento,” dijo Ernesto con "
    "una sonrisa, aunque sus ojos brillaban con un destello de misterio.\n\n"

    "Ana no estaba convencida de que fuera solo un mito. Esa misma tarde, decidió visitar la cascada por "
    "sí misma. Caminó durante horas, atravesando senderos llenos de árboles tropicales y arbustos que "
    "parecían susurrar entre sí. Cuando finalmente llegó, se encontró con un espectáculo asombroso: la "
    "cascada caía desde lo alto de una roca gigante, formando un arcoíris en el aire. A su alrededor, "
    "crecían interminables filas de bananos, todos cargados de frutas amarillas que brillaban como si "
    "fueran de oro.\n\n"

    "Mientras Ana observaba las bananas, notó algo extraño. Una de ellas parecía moverse ligeramente, "
    "como si estuviera viva. Se acercó con cautela y, para su sorpresa, la banana le habló. “Hola, Ana,” "
    "dijo la banana con una voz pequeña pero clara.\n\n"

    "Ana parpadeó, pensando que tal vez el cansancio le estaba jugando una mala pasada. “¿Me estás hablando?” "
    "preguntó.\n\n"

    "“Por supuesto,” respondió la banana. “Soy Bananín, el guardián de este lugar. Solo aquellos con un corazón "
    "puro pueden escucharme. Y parece que tú eres una de esas personas especiales.”\n\n"

    "Ana, aunque atónita, decidió escuchar al pequeño guardián. Bananín le explicó que las bananas de la región "
    "eran tan especiales porque estaban conectadas con la energía de la naturaleza. Cada banana contenía no solo "
    "nutrientes, sino también fragmentos de magia que hacían felices a quienes las comían. Sin embargo, Bananín "
    "también le advirtió que esta magia podía desaparecer si las personas comenzaban a explotar los recursos sin cuidado.\n\n"

    "“Debes ayudar a proteger este lugar,” dijo Bananín. “Si los humanos siguen cultivando bananas sin respetar la tierra, "
    "perderemos nuestra magia.”\n\n"

    "Ana, inspirada por las palabras del guardián, decidió actuar. Regresó al pueblo y comenzó a organizar reuniones con "
    "los agricultores, explicándoles la importancia de cuidar el suelo y el agua. Muchos al principio se mostraron "
    "escépticos, pero cuando vieron que las bananas que crecían con prácticas sostenibles eran aún más deliciosas, "
    "comenzaron a seguir las recomendaciones de Ana.\n\n"

    "Con el tiempo, el pueblo se convirtió en un ejemplo de armonía entre los humanos y la naturaleza. Las bananas del lugar "
    "no solo continuaron siendo famosas por su sabor, sino que también se convirtieron en un símbolo de cómo el respeto por "
    "el medio ambiente puede traer prosperidad.";


void reduction_op(void *invec, void *inoutvec) {
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

    double start_time = MPI_Wtime();

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

    if(mpi_size == 4){
        if (mpi_rank == 0 || mpi_rank == 1) {
            // Procesos 0 y 1: enviar y recibir entre sí
            if (mpi_rank == 0) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix, intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        } else if (mpi_rank == 2 || mpi_rank == 3) {
            // Procesos 2 y 3: enviar y recibir entre sí
            if (mpi_rank == 2) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix,intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 2, 0, MPI_COMM_WORLD);
            }
        }

        // Reducir los resultados de los pares de procesos
        if (mpi_rank == 0) {
            MPI_Recv(&final_matrix, N * M, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(&intermediate_matrix, &final_matrix);
            cout << "Cantidad de matcheos: " << final_matrix[1][q0] << endl; 
            double end_time = MPI_Wtime();
            cout << "Tiempo transcurrido: " << (end_time - start_time) << " segundos" << std::endl;
        } else if (mpi_rank == 2) {
            MPI_Send(&intermediate_matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    else if(mpi_size == 8){

        // Nivel 2
        if (mpi_rank == 0 || mpi_rank == 1) {
            // Procesos 0 y 1: enviar y recibir entre sí
            if (mpi_rank == 0) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix, intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        } else if (mpi_rank == 2 || mpi_rank == 3) {
            // Procesos 2 y 3: enviar y recibir entre sí
            if (mpi_rank == 2) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix,intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 2, 0, MPI_COMM_WORLD);
            }
        } else if (mpi_rank == 4 || mpi_rank == 5) {
            // Procesos 4 y 5: enviar y recibir entre sí
            if (mpi_rank == 4) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 5, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix, intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 4, 0, MPI_COMM_WORLD);
            }
        } else if (mpi_rank == 6 || mpi_rank == 7) {
            // Procesos 6 y 7: enviar y recibir entre sí
            if (mpi_rank == 6) {
                MPI_Recv(intermediate_matrix, N * M, MPI_INT, 7, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(matrix, intermediate_matrix);

            } else {
                MPI_Send(&matrix[0][0], N * M, MPI_INT, 6, 0, MPI_COMM_WORLD);
            }
        }

        // Nivel 1
        if (mpi_rank == 0 || mpi_rank == 2) {
            // Procesos 0 y 2: enviar y recibir entre sí
            if (mpi_rank == 0) {
                MPI_Recv(final_matrix, N * M, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(intermediate_matrix, final_matrix);
            } else {
                MPI_Send(&intermediate_matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        } else if (mpi_rank == 4 || mpi_rank == 6) {
            // Procesos 4 y 6: enviar y recibir entre sí
            if (mpi_rank == 4) {
                MPI_Recv(final_matrix, N * M, MPI_INT, 6, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                reduction_op(intermediate_matrix, final_matrix);
            } else {
                MPI_Send(&intermediate_matrix[0][0], N * M, MPI_INT, 4, 0, MPI_COMM_WORLD);
            }
        }

        // Nivel 0
        if (mpi_rank == 0) {
            // Procesos 0 y 4: enviar y recibir entre sí
            MPI_Recv(&final_matrix, N * M, MPI_INT, 4, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            reduction_op(&final_matrix, &final_matrix);
            cout << "Cantidad de matcheos: " << final_matrix[1][q0] << endl;
            double end_time = MPI_Wtime();
            cout << "Tiempo transcurrido: " << (end_time - start_time) << " segundos" << std::endl; 
        } else if (mpi_rank == 4) {
            MPI_Send(&final_matrix[0][0], N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}