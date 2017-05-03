#include <iostream>
#include "Evolution.h"

using namespace std;

#define STRINGIFY(x) #x
#define STRINGIFYMACRO(y) STRINGIFY(y)

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


    cout << seed<< ";";
    cout << STRINGIFYMACRO(MUTATIONPROB) << ";";
    cout << STRINGIFYMACRO(MUTATEDGENES) << ";";
    cout << STRINGIFYMACRO(CROSSOVERPROBABILITY) << ";";
    cout << STRINGIFYMACRO(TOURNAMENTSIZE) << ";";
    cout << STRINGIFYMACRO(POPSIZE) << ";";
    cout << STRINGIFYMACRO(MAXGENERATIONS) << ";";
    cout << STRINGIFYMACRO(STATES) << ";";
    cout << STRINGIFYMACRO(NEIGHBOURHOOD) << ";";
    cout << STRINGIFYMACRO(SIMULATIONSTEPS) << ";";
    cout << STRINGIFYMACRO(SIZEOFCA) << ";";
    cout << STRINGIFYMACRO(MINRULES) << ";";
    cout << STRINGIFYMACRO(MAXRULES) << ";";


    cout << endl;
    return 0;

}