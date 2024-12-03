#include <iostream>
#ifdef B_WORD
#include "../automatas/b_word.h"
string automata = "b_";
#endif
#ifdef BANANA_WORD
#include "../automatas/banana_word.h"
string automata = "banana_";
#endif
#ifdef PARALLEL_WORD
#include "../automatas/parallel_word.h"
string automata = "parallel_";
#endif

#include <omp.h>
#include <fstream>

using namespace std;
//Read txt file
string get_text(string filename, int NUM_THREADS){
    string text;
    ifstream file(filename);
    if (file.is_open()){
        string line;
        while (getline(file, line)){
            text += line;
        }
        file.close();
    }
    //Si no es multiplo, no funciona bien
    if(text.size() % NUM_THREADS != 0){
        int padding = NUM_THREADS - (text.size() % NUM_THREADS);
        for (int i = 0; i < padding; i++){
            text += "a";
        }
    }
    cout << "Size of text: " << text.size() << endl;
    return text;
}

double run_parallel(const string& T, int NUM_THREADS){
    int n = T.size();
    omp_set_num_threads(NUM_THREADS);
    vector<vector<int>> L(NUM_THREADS,vector<int>(NUM_STATES));
    vector<vector<int>> R(NUM_THREADS,vector<int>(NUM_STATES, 0));
    double start_time = omp_get_wtime();
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
    return (end_time - start_time)*1e6;
}

int main(int argc, char **argv) {
    // vector<string> textos = {"10k","100k","500k","1M"};
    vector<string> textos = {"10k","100k","500k","1M"};
    vector<int> p = {2,4,16,32};
    string output_file = "results_trivial_omp.txt";
    ofstream output(output_file);
    for(string texto : textos){
        for (int num_threads : p){
            output << "N: " << texto << " Threads: " << num_threads << endl;
            // string T = get_text("../textos/banana_" + texto + ".txt",num_threads);
            string T = get_text("../textos/" + automata + texto + ".txt",num_threads);
            double count = 0;
            cout << "P: " << num_threads << endl;
            for (int i = 0;  i < 10; i++) {
                count += run_parallel(T,num_threads);
            }
            count/=10;
            output << "Average time: " << count << " microseconds" << endl;
        }
    }
    output.close();

    return 0;
}
