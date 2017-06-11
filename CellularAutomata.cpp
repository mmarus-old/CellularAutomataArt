//
// Created by archie on 4/17/17.
//

#include <fstream>
#include <iostream>
#include <cmath>
#include <map>
#include "CellularAutomata.h"
#include "NumberGenerator.h"


CellularAutomata::CellularAutomata() {

}

void CellularAutomata::initializeCA() {
  changedStates = 0;
  isCrossedOver = false;
  rulesVector.resize(maxValueFromFunction);
  currentMap.resize(heigth, vector<int>(width, 0));
  newMap.resize(heigth, vector<int>(width, 0));
  oldStates.resize(SIMULATIONSTEPS, newMap);
  mapWithVisitedStates.resize(STATES, newMap);
}

void CellularAutomata::setFirstState() {
  changedStates = 0;
  isCrossedOver = false;
  std::fill(currentMap.begin(), currentMap.end(), vector<int>(width, 0));
  std::fill(newMap.begin(), newMap.end(), vector<int>(width, 0));
  std::fill(mapWithVisitedStates.begin(), mapWithVisitedStates.end(), newMap);

  currentMap[heigth / 2][width / 2] = 1;
  currentMap[heigth / 2 - 1][width / 2] = 1;
  currentMap[heigth / 2][width / 2 - 1] = 1;
  currentMap[heigth / 2 + 1][width / 2] = 1;
  currentMap[heigth / 2][width / 2 + 1] = 1;


}

void CellularAutomata::runSimulation() {
  int oldChangedStates;
  stepsWithChangedStates = 0;


  for (int i = 0; i < simulationSteps; ++i) {
    oldChangedStates = changedStates;
    develop();
    if (oldChangedStates == changedStates)
      break;
    else
      stepsWithChangedStates++;
    if (isCrossedOver)
      break;
  }
}

void CellularAutomata::develop() {
  for (int i = 1; i < heigth - 1; ++i) {
    for (int j = 1; j < width - 1; ++j) {
      valueOfcenterOfNeigh = currentMap[i][j];
      rowOfCenterOfNeigh = i;
      colOfCenterOfNeigh = j;

      int newValue = updateFunction();
      newMap[i][j] = newValue;
      mapWithVisitedStates[newValue][i][j] = 1;

      if ((i == width - 2 || j == width - 2 || i == 1 || j == 1) && newMap[i][j] != 0) {
        isCrossedOver = true;
        return;
      }
    }
  }
  oldStates.push_back(currentMap);
  swapMaps();
}

void CellularAutomata::swapMaps() {
  currentMap.swap(newMap);
}


int CellularAutomata::updateFunction() {
  int result = 0;
  int index = mathFunction();

  if (index < rulesVector.size()) {
    result = rulesVector[index];
    if (result != valueOfcenterOfNeigh)
      changedStates++;
  } else {
    result = valueOfcenterOfNeigh;
  }

  return result;
}

int CellularAutomata::mathFunction() {
  int result = 0;

  if (neighbourhoodSize == 5) {
    result += currentMap[rowOfCenterOfNeigh - 1][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh - 1];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh + 1][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh + 1];
  } else if (neighbourhoodSize == 9) {
    result += currentMap[rowOfCenterOfNeigh - 1][colOfCenterOfNeigh - 1];
    result += currentMap[rowOfCenterOfNeigh - 1][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh - 1][colOfCenterOfNeigh + 1];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh - 1];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh + 1];
    result += currentMap[rowOfCenterOfNeigh + 1][colOfCenterOfNeigh - 1];
    result += currentMap[rowOfCenterOfNeigh + 1][colOfCenterOfNeigh];
    result += currentMap[rowOfCenterOfNeigh + 1][colOfCenterOfNeigh + 1];
  }


  return result;
}

int CellularAutomata::mathFunction(const vector<int> &cellNeighbours) {
  int result = 0;
  for (int i = 0; i < cellNeighbours.size(); ++i) {
    result += cellNeighbours[i];
  }
  return result;
}

int CellularAutomata::updateFunction(const vector<int> &cellNeighbours) {
  int result = 0;
  int index = mathFunction(cellNeighbours);

  if (index < rulesVector.size()) {
    result = rulesVector[index];
    if (result != cellNeighbours[neighbourhoodSize / 2])
      changedStates++;
  } else {
    result = cellNeighbours[neighbourhoodSize / 2];
  }

  return result;
}


void CellularAutomata::exportCurrentState(string filename) {
  ofstream fout(filename, std::ofstream::out);

  for (int i = 0; i < heigth; ++i) {
    fout << currentMap[i][0];
    for (int j = 1; j < width; ++j) {
      fout << " " << currentMap[i][j];
    }
    fout << endl;
  }
  fout.close();

}


void CellularAutomata::exportRules(string filename) {
  unsigned long long count = 0;
  ofstream fout(filename, std::ofstream::out);
  fout << states << endl;
  fout << neighbourhoodSize << endl;
  for (int i = 0; i < rulesVector.size(); ++i) {
    fout << i << " " << rulesVector[i] << endl;
    count++;
  }

  cerr << "# of rules = " << count << endl;
  fout.close();
}

bool CellularAutomata::isDead() {
  for (int i = 0; i < heigth; ++i) {
    for (int j = 0; j < width; ++j) {
      if (currentMap[i][j] != 0)
        return false;
    }
  }
  return true;
}


