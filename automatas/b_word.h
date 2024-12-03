#include "base.h"
#define NUM_STATES 2
TransitionTable Tt = {
    {0, 1},
    {0, 1}
};

States F = {1}; // Final states

int q0 = 0;

unordered_map <char, int> sigma = {
    {'a', 0},
    {'b', 1}
};