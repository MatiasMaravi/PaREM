#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <omp.h> // OpenMP
#define NUM_THREADS 4

using namespace std;

typedef vector<vector<int>> TransitionTable;
typedef set<int> FinalStates;
typedef set<int> States;
typedef vector<vector<int>> Mapping;

TransitionTable Tt = {
    {0, 1, 0}, // δA(0, símbolo)
    {2, 1, 0}, // δA(1, símbolo)
    {0, 1, 3}, // δA(2, símbolo)
    {4, 1, 0}, // δA(3, símbolo)
    {0, 1, 5}, // δA(4, símbolo)
    {6, 1, 0}, // δA(5, símbolo)
    {0, 1, 0}  // δA(6, símbolo)
};
FinalStates F = {6};
States Q = {0, 1, 2, 3, 4, 5, 6};
string T = "abananabananabaabananabananaababanan";
unordered_map <char, int> sigma = {
    {'a', 0},
    {'b', 1},
    {'n', 2}
};
Mapping L(Q.size(), vector<int>(NUM_THREADS));
Mapping R(Q.size(), vector<int>(NUM_THREADS));

bool PaREM(TransitionTable Tt, FinalStates F, string T, int q0, Mapping& L, Mapping& R) {
    int p = NUM_THREADS;
    int n = T.size();
    
    #pragma omp parallel
    {
        int P_i = omp_get_thread_num();
        int j = P_i * (n / p);
        int end_position = (P_i + 1) * (n / p) - 1;
        if (P_i == p - 1) end_position = n - 1; // Asegurarse que el último hilo cubra hasta el final
        
        // Inicializar L y R locales para cada hilo
        vector<int> L_local(Q.size()), R_local(Q.size(), 0);
        
        for (int k = 0; k < Q.size(); k++) {
            L_local[k] = k;
        }

        // Evaluar las transiciones para cada símbolo en la sección de la cadena asignada
        while (j <= end_position) {
            for (int k = 0; k < Q.size(); k++) {
                L_local[k] = Tt[L_local[k]][sigma[T[j]]]; // Evaluar transición
                if (Q.find(L_local[k]) != Q.end()) {
                    R_local[k]++;
                }
            }
            j++;
        }

        // Copiar valores locales a las matrices globales al final de la región paralela
        #pragma omp critical
        {
            for (int k = 0; k < Q.size(); k++) {
                L[P_i][k] = L_local[k];
                R[P_i][k] = R_local[k];
            }
        }
    }

    // Reducir los valores de L y R en serie
    int L_temp = q0;
    int R_temp = 0;

    for (int k = 0; k < NUM_THREADS; k++) {
        R_temp += R[k][L_temp];
        L_temp = L[k][L_temp];
    }

    // Actualizar valores finales
    R[0][q0] = R_temp;
    L[0][q0] = L_temp;

    if (F.find(L[0][q0]) != F.end()) {
        cout << "La cadena es aceptada" << endl;
        return true;
    } else {
        cout << "La cadena no es aceptada" << endl;
        return false;
    }
}

int main() {
    omp_set_num_threads(NUM_THREADS);
    PaREM(Tt, F, T, 0, L, R);
    return 0;
}
