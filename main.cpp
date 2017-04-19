#include <iostream>
#include "CellularAutomata.h"
#include "NumberGenerator.h"
#include "Chromosome.h"
#include "Evolution.h"


using namespace std;



int main() {
    time_t seed = time(0);
    srand(seed);

    Evolution evolution;
    evolution.initialize();
    evolution.evolve();
    evolution.exportChromosome();
//
//    Chromosome chromosome;
//    chromosome.initializeRandomRules();
//    chromosome.initializeCa();
//    chromosome.mutateRandomRule();
//    chromosome.calculateFittness();
//    cout << "fittness = " << chromosome.getFittness() << endl;
//    chromosome.exportCA();
//
//
    cout << "Seed = " << seed << endl;
    return 0;
}