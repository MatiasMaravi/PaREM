#include <iostream>
#include "../automatas/banana_word.h"
#include <mpi.h>
using std::cout;
using std::endl;

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
        double end_time = MPI_Wtime();
        cout << "Tiempo transcurrido: " << (end_time - start_time) << " segundos" << std::endl;
    }

    

    MPI_Finalize();
    return 0;
}
