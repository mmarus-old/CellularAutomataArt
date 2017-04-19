//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Chromosome.h"
#include <cmath>


using namespace std;

Chromosome::Chromosome() {
    maxRules = ipow( states, neighbourhoodSize);
}

void Chromosome::initializeRandomRules() {
    rulesKeys.resize(maxRules, vector<int>(neighbourhoodSize));

    while(rulesMap.size() < maxRules) {
        addRandomRule(rulesMap.size());
    }

    cout << "vygenerovany pocet pravidiel = "<< rulesMap.size() << endl;
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
    ca.setRulesMap(rulesMap);
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
        }
    }
    fittness += ca.changedStates;
    evaluate = false;
}

void Chromosome::initializeCa() {
    ca.initializeCA();
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
    int indexNumber = urandom(0, maxRules);
//    cout << "erasing " << indexNumber << "th key" << endl;

    vector<int> keyToRemove = rulesKeys[indexNumber];
    rulesMap.erase(keyToRemove);

    addRandomRule(indexNumber);
}

vector<int> Chromosome::generateRandomKey() {
    vector<int> randomKey(neighbourhoodSize);

    for (int j = 0; j < randomKey.size(); ++j) {
        randomKey[j] = urandom(0, states);
    }
    return randomKey;
}

void Chromosome::addRandomRule(int index) {
    unsigned long oldSize = rulesMap.size();
    vector<int> key;
    int random = urandom(0, states);

    while(oldSize == rulesMap.size()){
        key = generateRandomKey();

        if(rulesMap.find(key) == rulesMap.end()){
            while(random==key[neighbourhoodSize/2]){
                random = urandom(0, states);
            }
            rulesMap[key] = random;
        }
    }

    rulesKeys[index] = key;

//    cout << "key #" << index << " key = ";
//    for (int i = 0; i < key.size(); ++i) {
//        cout << key[i] << " ";
//    }
//    cout << " value= " << random << endl;
}

int Chromosome::ipow(int base, int exp) {
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
