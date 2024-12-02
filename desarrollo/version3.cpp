#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <omp.h>
#include <fstream>
#define NUM_STATES 9
#define NUM_THREADS 4
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
vector<vector<int>> Tt  = {
    {0, 1, 0},
    {2, 1, 0},
    {0, 1, 3},
    {4, 1, 0},
    {0, 1, 5},
    {6, 1, 0},
    {0, 1, 0}
};

string T = get_text("../textos/banana_100k.txt");
set<int> F = {6}; // Final state
int q0 = 0;

unordered_map <char, int> sigma = {
    {'a', 0},
    {'b', 1},
    {'n', 2}
};


int main(int argc, char **argv) {
    int n = T.size();
    omp_set_num_threads(NUM_THREADS);
    vector<vector<int>> L(NUM_THREADS,vector<int>(NUM_STATES));
    vector<vector<int>> R(NUM_THREADS,vector<int>(NUM_STATES, 0));
    
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
    cout << "Cantidad de veces que la cadena es aceptada: " << R[0][q0] << endl;

    return 0;
}
