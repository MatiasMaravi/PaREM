
#include "base.h"

TransitionTable Tt = {
    {0, 1, 0},
    {2, 1, 0},
    {0, 1, 3},
    {4, 1, 0},
    {0, 1, 5},
    {6, 1, 0},
    {0, 1, 0}
};

States Q = {0, 1, 2, 3, 4, 5, 6}; // States
States F = {6}; // Final states

const int M = 7; // Number of states
const int N = 2; // L and R

int q0 = 0;

string T = "abananabananabaabananabananaababanan";

unordered_map <char, int> sigma = {
    {'a', 0},
    {'b', 1},
    {'n', 2}
};