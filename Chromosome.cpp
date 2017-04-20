//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Chromosome.h"
#include <cmath>
#include <algorithm>


using namespace std;

Chromosome::Chromosome() {
}

void Chromosome::initialize() {
    ca.initializeCA();

    for (int i = 0; i < ca.rulesVector.size(); ++i) {
        addRandomRule(i);
    }
}

unsigned Chromosome::urandom(unsigned low, unsigned high)
{
    high = high -1;
    return rand() % (high - low + 1) + low;
}

void Chromosome::calculateFittness() {
    if(!evaluate)
        return;
    fittness = 0;

    ca.setFirstState();
    ca.runSimulation();

    if(ca.isCrossedOver || ca.isDead()){
        fittness = 0;
        return;
    }

    for (int i = 0; i < ca.heigth; ++i) {
        for (int j = 0; j < ca.width; ++j) {
            if(ca.currentMap[i][j] !=0)
                fittness++;

            if((ca.oldStates.size() >= SIMULATIONSTEPS-1)
               && (ca.oldStates[ca.oldStates.size()-1][i][j] != ca.oldStates[ca.oldStates.size()-2][i][j] &&
                ca.oldStates[ca.oldStates.size()-1][i][j] != ca.oldStates[ca.oldStates.size()-3][i][j]))
                fittness++;

        }
    }
//    cout << "fit " << fittness << endl;
    fittness += ca.stepsWithChangedStates;
    fittness += ca.changedStates;
    evaluate = false;
}

void Chromosome::exportCA(){
    ca.exportCurrentState("bicas/caEND.cas");
    ca.setFirstState();
    ca.exportCurrentState("bicas/ca.cas");
    ca.exportRules("bicas/ca.tab");
}

int Chromosome::getFittness() {
    return fittness;
}

void Chromosome::mutateRandomRule() {
    int indexNumber = urandom(0, ca.rulesVector.size());
    addRandomRule(indexNumber);
}

void Chromosome::addRandomRule(int index) {
    if(index < ca.rulesVector.size())
        ca.rulesVector[index] = urandom(0, STATES);
}
