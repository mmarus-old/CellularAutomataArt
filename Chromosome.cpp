//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Chromosome.h"
#include <cmath>
#include <algorithm>
#include <omp.h>


using namespace std;

Chromosome::Chromosome() {
}

void Chromosome::initialize() {
  ca.initializeCA();

  for (int i = 0; i < ca.rulesVector.size(); ++i) {
    addRandomRule(i);
  }
}

unsigned Chromosome::urandom(unsigned low, unsigned high) {
  high = high - 1;
  return rand() % (high - low + 1) + low;
}

void Chromosome::calculateFittness() {
  if (!evaluate)
    return;
  fittness = 0;

  ca.setFirstState();
  ca.runSimulation();

  if (ca.isCrossedOver || ca.isDead()) {
    fittness = 0;
    return;
  }

  for (int i = 0; i < ca.heigth; ++i) {
    for (int j = 0; j < ca.width; ++j) {
      int usedStates = 0;
      for (int state = 0; state < STATES; state++) {
        usedStates += ca.mapWithVisitedStates[state][i][j];
      }
      fittness += usedStates * usedStates;
    }
  }

//    fittness += ca.stepsWithChangedStates*2;
//    fittness += ca.changedStates;
  evaluate = false;
}

void Chromosome::exportCA(string suffix) {
  string dir  = "bicas/";
  string number = FILENAME;

  ca.exportCurrentState(dir + number + "caEND"+suffix+".cas");
  ca.setFirstState();
  ca.exportCurrentState(dir + number + "ca"+suffix+".cas");
  ca.exportRules(dir + number + "ca"+suffix+".tab");
}

int Chromosome::getFittness() {
  return fittness;
}

void Chromosome::mutateRandomRule() {
  int indexNumber = urandom(0, ca.rulesVector.size());
  addRandomRule(indexNumber);
}

void Chromosome::addRandomRule(int index) {
  if (index < ca.rulesVector.size())
    ca.rulesVector[index] = urandom(0, STATES);
}
