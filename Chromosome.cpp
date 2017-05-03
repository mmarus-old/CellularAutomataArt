//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Chromosome.h"
#include <cmath>


using namespace std;

Chromosome::Chromosome() {
}

void Chromosome::initialize() {
  ca.initializeCA();

  rulesKeys.resize(maxRules, vector<int>(neighbourhoodSize));

  for (int i =0; i <maxRules; i++)
    addRandomNonEmptyRule();

  fittness = 0;
}

void Chromosome::addRandomRule() {
  unsigned long index = rulesMap.size();
  vector<int> key;

  key = generateRandomKey();

  if (rulesMap.find(key) == rulesMap.end()) {
    rulesMap[key] = urandom(0, states);
  }

  if(rulesMap.size() > rulesKeys.size())
    rulesKeys.resize(rulesMap.size());
  rulesKeys[index] = key;
}

void Chromosome::addRandomNonEmptyRule() {
  unsigned long index = rulesMap.size();
  vector<int> key;
  int random = urandom(0, states);


  key = generateRandomKey();

  if (rulesMap.find(key) == rulesMap.end()) {
    while(random==key[neighbourhoodSize/2]){
      random = urandom(0, states);
    }
    rulesMap[key] = random;
  }

  rulesKeys[index] = key;
}

vector<int> Chromosome::generateRandomKey() {
  vector<int> randomKey(neighbourhoodSize);

  for (int j = 0; j < randomKey.size(); ++j) {
    randomKey[j] = urandom(0, states);
  }
  return randomKey;
}


unsigned Chromosome::urandom(unsigned low, unsigned high) {
  high = high - 1;
  return rand() % (high - low + 1) + low;
}

void Chromosome::calculateFittness() {
  if (!evaluate)
    return;
  fittness = 0;

  ca.rulesMap = rulesMap;
  ca.setFirstState();
  ca.runSimulation();

  if (ca.isCrossedOver || ca.isDead()) {
    fittness = 0;
    return;
  }
  for (int i = 0; i < ca.heigth; ++i) {
    for (int j = 0; j < ca.width; ++j) {
      int usedStates = 0;
      for (int state = 1; state < STATES; state++) {
//        if(ca.mapWithVisitedStates[state][i][j] ==1)
//          cout << state <<" "<<i <<" "<< j << " = " <<ca.mapWithVisitedStates[state][i][j]<< endl;
        usedStates += ca.mapWithVisitedStates[state][i][j];
      }
      fittness += usedStates * usedStates;
    }
  }
  evaluate = false;
}

void Chromosome::exportCA() {
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

  vector<int> keyToRemove = rulesKeys[indexNumber];
  rulesMap.erase(keyToRemove);

  addRandomRule();
}


