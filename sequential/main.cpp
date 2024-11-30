#include <iostream>
#include "../automatas/banana_word.h"
#include <fstream>
#include <chrono>
using std::cout;
using std::endl;
using std::ifstream;


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
    if(text.size() % 4 != 0){
        int padding = 4 - (text.size() % 4);
        for (int i = 0; i < padding; i++){
            text += " ";
        }
    }
    cout << "Size of text: " << text.size() << endl;
    return text;
}


int main(){
    string T = get_text("../textos/banana_200k.txt");
    int contador = 0;
    int j = 0;
    int q = q0;
    int n = T.size();
    auto start = std::chrono::high_resolution_clock::now();
    while (j <= n){
        char c = T[j];
        q = Tt[q][sigma[c]];
        if (F.find(q) != F.end()){
            contador++;
        }
        j++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << endl;
    cout << "Cantidad de matcheos: " << contador << endl;
}