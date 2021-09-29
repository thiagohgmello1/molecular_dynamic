#include "headers/utils.h"


#define NORMALIZE (int)1073741824
#define A (int)883314861
#define B (int)453816693


float rand_num(int* seed){
    double norm = 0.5/(NORMALIZE);
    double random_num = 0;

    *seed = *seed * A + B;

    if (*seed < 0){
        *seed = (*seed + NORMALIZE) + NORMALIZE;
    }

    random_num = *seed * norm;
    return random_num;

}