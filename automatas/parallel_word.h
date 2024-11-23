#include "base.h"
TransitionTable Tt = {
    {1, 0, 0, 0, 0},
    {1, 2, 0, 0, 0},
    {1, 0, 3, 0, 0},
    {1, 4, 0, 0, 0},
    {1, 0, 0, 0, 5},
    {1, 0, 0, 0, 6},
    {1, 0, 0, 7, 0},
    {1, 0, 0, 0, 8},
    {1, 0, 0, 0, 0}
};
string T = "parallel";
States F = {8}; // Final states
int q0 = 0;
States Q = {0, 1, 2, 3, 4, 5, 6, 7, 8};
unordered_map <char, int> sigma = {
    {'p', 0},
    {'a', 1},
    {'r', 2},
    {'e', 3},
    {'l', 4}
};

