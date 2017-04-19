//
// Created by archie on 4/17/17.
//

#ifndef BIN_EVO_PROJEKT_CHROMOSOME_H
#define BIN_EVO_PROJEKT_CHROMOSOME_H

#include <map>
#include <vector>
#include "CellularAutomata.h"
#include "params.h"

using namespace std;

class Chromosome {

public:
    Chromosome();
    void calculateFittness();
    void exportCA();
    void initialize();
    void addRandomRule(int index);
    static unsigned urandom(unsigned int low, unsigned int high);
    bool evaluate = true;
    unsigned int fittness = 0;
    CellularAutomata ca;
    int getFittness();
    void mutateRandomRule();

};


#endif //BIN_EVO_PROJEKT_CHROMOSOME_H
