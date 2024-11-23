
#include "base.h"
#define NUM_STATES 7
TransitionTable Tt = {
    {0, 1, 0},
    {2, 1, 0},
    {0, 1, 3},
    {4, 1, 0},
    {0, 1, 5},
    {6, 1, 0},
    {0, 1, 0}
};

States F = {6}; // Final states

int q0 = 0;

unordered_map <char, int> sigma = {
    {'a', 0},
    {'b', 1},
    {'n', 2}
};