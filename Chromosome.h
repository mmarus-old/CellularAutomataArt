//
// Created by archie on 4/17/17.
//

#ifndef BIN_EVO_PROJEKT_CHROMOSOME_H
#define BIN_EVO_PROJEKT_CHROMOSOME_H

#include <map>
#include <vector>
#include "CellularAutomata.h"
using namespace std;

class Chromosome {

public:
    Chromosome();
    void calculateFittness();
    void exportCA();
    void initializeRandomRules();
    vector<int> generateRandomKey();
    void addRandomRule(int index);
    void initializeCa();
    static unsigned urandom(unsigned int low, unsigned int high);
    int ipow(int base, int exp);

    bool evaluate = true;
    map<vector<int>,int> rulesMap;
    vector<vector<int>> rulesKeys;
    unsigned int fittness = 0;
    unsigned int maxRules; //Velkost chromozomu
    int neighbourhoodSize = NEIGHBOURHOOD; // equal
    int states = STATES;

    CellularAutomata ca;

    int getFittness();

    void mutateRandomRule();
};


#endif //BIN_EVO_PROJEKT_CHROMOSOME_H
