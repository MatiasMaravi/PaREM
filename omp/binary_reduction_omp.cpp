#include <iostream>
#include "../automatas/banana_word.h"
#include <cmath>
#include <omp.h>
#include <fstream>
#define NUM_THREADS 16
using namespace std;

//Read txt file
string get_text(string filename){
    string text;
    ifstream file(filename);
    if (file.is_open()){
        string line;
        while (getline(file, line)){
            text += line;
        }
        file.close();
    }
    //Si no es multiplo de 4, agregar padding
    if(text.size() % NUM_THREADS != 0){
        int padding = NUM_THREADS - (text.size() % NUM_THREADS);
        for (int i = 0; i < padding; i++){
            text += "a";
        }
    }
    cout << "Size of text: " << text.size() << endl;
    return text;
}

double run_parallel(const string& T){
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
            L[rank][q0] = Tt[L[rank][q0]][sigma[T[j]]];
            if (F.find(L[rank][q0]) != F.end()) {
                R[rank][q0]++;
            }
            j++;
        }
        #pragma omp barrier
        for(int m = 1; m <= ceil(log2(size));m++){
            if(rank % (int)pow(2,m) == 0 && (rank + (int)pow(2,m-1)) < size){
                
                R[rank][q0] += R[rank + (int)pow(2,m-1)][L[rank][q0]];
                L[rank][q0] = L[rank + (int)pow(2,m-1)][L[rank][q0]];
            
            }
        }
    }
    double end_time = omp_get_wtime();
    cout << "Cantidad de veces que la cadena es aceptada: " << R[0][q0] << endl;
    return (end_time - start_time)*1e6;
}

int main(int argc, char **argv) {

    string T = get_text("../textos/banana_200k.txt");
    double count = 0;
    for (int i = 0;  i < 10; i++) {
        count += run_parallel(T);
    }
    count/=10;
    cout << "Time: " << count << " microseconds" << endl;

    return 0;
}
