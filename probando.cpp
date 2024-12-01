#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <omp.h>

using namespace std;

// Transition table type alias
using TransitionTable = unordered_map<char, unordered_map<char, unordered_set<char>>>;

// Function to compute possible initial and last states for a processing unit
void computeStates(const string& input, int start, int end, const TransitionTable& Tt, 
                   unordered_set<char>& S, unordered_set<char>& L) {
    // Compute possible initial states
    for (const auto& [state, transitions] : Tt) {
        if (transitions.count(input[start])) {
            S.insert(state);
        }
    }

    // Compute possible last states
    for (const auto& [state, transitions] : Tt) {
        if (transitions.count(input[end])) {
            L.insert(state);
        }
    }
}

// Main REM computation for a processing unit
void processChunk(const string& input, int start, int end, const TransitionTable& Tt, 
                  const unordered_set<char>& F, const unordered_set<char>& R, vector<unordered_set<char>>& Rr) {
    for (const auto& r : R) {
        char currentState = r;
        for (int i = start; i <= end; ++i) {
            char symbol = input[i];
            if (Tt.at(currentState).count(symbol)) {
                currentState = *Tt.at(currentState).at(symbol).begin();
                if (F.count(currentState)) {
                    Rr[i].insert(currentState);
                }
            }
        }
    }
}

// PaREM algorithm implementation with OpenMP
void PaREM(const string& T, const TransitionTable& Tt, const unordered_set<char>& F, int p) {
    int chunkSize = T.size() / p;
    vector<unordered_set<char>> S(p), L(p), R(p);
    vector<vector<unordered_set<char>>> I(p, vector<unordered_set<char>>(T.size()));

    // Parallel computation of S and L using OpenMP
    #pragma omp parallel for num_threads(p)
    for (int i = 0; i < p; ++i) {
        int start = i * chunkSize;
        int end = (i == p - 1) ? T.size() - 1 : (i + 1) * chunkSize - 1;
        computeStates(T, start, end, Tt, S[i], L[i]);
    }

    // Compute intersection R = S ∩ L
    #pragma omp parallel for num_threads(p)
    for (int i = 0; i < p; ++i) {
        for (const auto& s : S[i]) {
            if (L[i].count(s)) {
                R[i].insert(s);
            }
        }
    }

    // Parallel processing of each chunk
    #pragma omp parallel for num_threads(p)
    for (int i = 0; i < p; ++i) {
        int start = i * chunkSize;
        int end = (i == p - 1) ? T.size() - 1 : (i + 1) * chunkSize - 1;
        processChunk(T, start, end, Tt, F, R[i], I[i]);
    }

    // Combine results (Optional: implement reduction logic here based on your use case)
    // Example: printing the results
    for (int i = 0; i < p; ++i) {
        cout << "Processor " << i << ": ";
        for (const auto& state_set : I[i]) {
            for (const auto& state : state_set) {
                cout << state << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    // Example input
    string T = "plaraparallelapareparaplplaraparallelaparallelapl";
    int p = 4; // Number of processing units

    // Transition table (example)
    TransitionTable Tt = {
        {'0', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'0'}}, {'e', {'0'}}}},
        {'1', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'2'}}, {'l', {'0'}}, {'e', {'0'}}}},
        {'2', {{'p', {'1'}}, {'r', {'3'}}, {'a', {'0'}}, {'l', {'0'}}, {'e', {'0'}}}},
        {'3', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'4'}}, {'l', {'0'}}, {'e', {'0'}}}},
        {'4', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'5'}}, {'e', {'0'}}}},
        {'5', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'6'}}, {'e', {'0'}}}},
        {'6', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'0'}}, {'e', {'7'}}}},
        {'7', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'8'}}, {'e', {'0'}}}},
        {'8', {{'p', {'1'}}, {'r', {'0'}}, {'a', {'0'}}, {'l', {'0'}}, {'e', {'0'}}}}
    };

    // Final states
    unordered_set<char> F = {'8'};

    // Run the algorithm
    PaREM(T, Tt, F, p);

    return 0;
}
