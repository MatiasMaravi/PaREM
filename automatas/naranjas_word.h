#include "base.h"
#define NUM_STATES 8
TransitionTable Tt = {
    {1, 0, 0, 0},
    {1, 2, 0, 0},
    {1, 0, 3, 0},
    {1, 4, 0, 0},
    {5, 0, 0, 0},
    {1, 0, 0, 6},
    {1, 7, 0, 0},
    {1, 0, 0, 0}
};

States F = {7}; // Final states
int q0 = 0;
unordered_map <char, int> sigma = {
    {'n', 0},
    {'a', 1},
    {'r', 2},
    {'j', 3}
};
