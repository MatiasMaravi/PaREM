#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <omp.h>

// Definiciones para simplificar el código
using TransitionTable = std::unordered_map<int, std::unordered_map<char, int>>;
using StateSet = std::unordered_set<int>;

// Función para procesar un segmento del input
std::vector<int> process_segment(
    const TransitionTable& Tt, 
    const StateSet& F, 
    const std::string& pi_input, 
    const std::vector<int>& q_states
) {
    StateSet S, L;
    std::vector<int> result;

    // Buscar posibles estados iniciales
    for (int q : q_states) {
        if (Tt.count(q) && Tt.at(q).count(pi_input.front())) {
            S.insert(q);
        }
    }

    // Buscar posibles estados finales
    for (int q : q_states) {
        if (Tt.count(q) && Tt.at(q).count(pi_input.back())) {
            L.insert(Tt.at(q).at(pi_input.back()));
        }
    }

    // Intersección de estados iniciales y finales
    StateSet R;
    for (int state : S) {
        if (L.count(state)) {
            R.insert(state);
        }
    }

    // Procesar estados válidos
    for (int r : R) {
        int current_state = r;
        for (char c : pi_input) {
            if (Tt.count(current_state) && Tt.at(current_state).count(c)) {
                current_state = Tt.at(current_state).at(c);
                if (F.count(current_state)) {
                    result.push_back(current_state);
                }
            }
        }
    }

    return result;
}

int main() {
    // Tabla de transición (Tt)
    TransitionTable Tt = {
        {0, {{'a', 1}, {'b', 2}}},
        {1, {{'a', 1}, {'b', 3}}},
        {2, {{'a', 3}}},
        {3, {{'b', 3}}}
    };

    // Conjunto de estados finales (F)
    StateSet F = {3};

    // Entrada y parámetros
    std::string input_text = "abbaabbab";
    int num_threads = 4;

    // Dividir el input en segmentos
    int n = input_text.length();
    int chunk_size = n / num_threads;

    std::vector<std::vector<int>> results(num_threads);
    std::vector<int> q_states = {0, 1, 2, 3};  // Estados posibles

    // Procesamiento paralelo con OpenMP
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; ++i) {
        int start_pos = i * chunk_size;
        int end_pos = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        std::string pi_input = input_text.substr(start_pos, end_pos - start_pos);

        results[i] = process_segment(Tt, F, pi_input, q_states);
    }

    // Combinar resultados
    std::vector<int> final_result;
    for (const auto& partial_result : results) {
        final_result.insert(final_result.end(), partial_result.begin(), partial_result.end());
    }

    // Imprimir resultados
    std::cout << "Estados finales alcanzados: ";
    for (int state : final_result) {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    return 0;
}