#include "../../Include/Utils/CommonUtils.h"
#include <random>

/* given a start and an end range, generate a random number.
*/
int getRandomAmount(int start, int end){
    /* At first, the std::random_device object should be initialized. It produces non-determini
     * stic random bits for random engine seeding, which is crucial to avoid producing the same 
     * number sequences. Here we use std::default_random_engine to generate pseudo-random values, 
     * but you can declare specific algorithm engine. Next, we initialize a uniform distribution
     * and pass min/max values as optional arguments.
    */
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(start, end);
    return distr(eng);
}