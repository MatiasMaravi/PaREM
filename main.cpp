#include <iostream>
#include <vector>

#include <omp.h> //OpenMP

using namespace std;

typedef vector<vector<int>> TransitionTable;
typedef vector<int> FinalStates;


TransitionTable Tt = {
   //p  a  r  e  l
    {1, 0, 0, 0, 0}, // δA(0, símbolo)
    {1, 2, 0, 0, 0}, // δA(1, símbolo)
    {1, 0, 3, 0, 0}, // δA(2, símbolo)
    {1, 0, 0, 4, 0}, // δA(3, símbolo)
    {1, 0, 0, 0, 5}, // δA(4, símbolo)
    {1, 0, 0, 0, 6}, // δA(5, símbolo)
    {1, 0, 7, 0, 0}, // δA(6, símbolo)
    {1, 0, 0, 0, 8}, // δA(7, símbolo)
    {1, 0, 0, 0, 0}  // δA(8, símbolo)
};
FinalStates F = {8};

string T = "plaraparallelapareparapl";


bool PaREM(TransitionTable Tt, // Tabla de transiciones
            FinalStates F, // Estados finales
            string T) // Cadena de entrada (input)
{
    int p = omp_get_num_threads();
    if (p == 1) {
        cout << "No se puede ejecutar en un solo hilo" << endl;
        return false;
    }
    return true;
}


int main(){
    cout << PaREM(Tt, F, T) << endl;
    
    return 0;
}