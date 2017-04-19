#include <iostream>
#include "Evolution.h"

using namespace std;

int main() {
    time_t seed = time(0);
    srand(seed);

    Evolution evolution;
    evolution.initialize();
    evolution.evolve();
    evolution.exportChromosome();

//    Chromosome c;
//    c.initialize();
//    c.calculateFittness();
//    c.exportCA();
//    cout << "fitness " << c.fittness << endl;

    cout << "Seed = " << seed << endl;

    return 0;
}