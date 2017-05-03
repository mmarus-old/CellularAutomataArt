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
  currentMap.resize(heigth, vector<int>(width, 0));
  newMap.resize(heigth, vector<int>(width, 0));
  oldStates.resize(SIMULATIONSTEPS, newMap);
  mapWithVisitedStates.resize(STATES, newMap);
}

void CellularAutomata::setFirstState() {
  changedStates = 0;
  isCrossedOver = false;
  for (int i = 0; i < heigth; ++i) {
    for (int j = 0; j < width; ++j) {
      currentMap[i][j] = 0;
    }
  }

  currentMap[heigth / 2][width / 2] = 1;
  currentMap[heigth / 2 - 1][width / 2] = 1;
  currentMap[heigth / 2][width / 2 - 1] = 1;
  currentMap[heigth / 2 + 1][width / 2] = 1;
  currentMap[heigth / 2][width / 2 + 1] = 1;

  std::fill(newMap.begin(), newMap.end(), vector<int>(width, 0));
  std::fill(mapWithVisitedStates.begin(), mapWithVisitedStates.end(), newMap);
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
  vector<int> cellNeighbours;

  for (int i = 1; i < heigth - 1; ++i) {
    for (int j = 1; j < width - 1; ++j) {
      cellNeighbours = getNeighbourhood(i,j);
      int newValue = updateFunction(cellNeighbours);
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

vector<int> CellularAutomata::getNeighbourhood(int row, int col) {
  if (neighbourhoodSize != 5 && neighbourhoodSize != 9)
    throw std::runtime_error("Bad neighbourhood");
  vector<int> neighbourhood;


  int i = 0;
  int j = 0;

  if(neighbourhoodSize == 5) {
    i = mod(row-1,21); j = col; //N
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = mod(col-1,21); //W
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = col; //C
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = mod(col+1,21); //E
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row+1,21); j = col; //South
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;
  } else if(neighbourhoodSize == 9) {
    i = mod(row-1,21); j = mod(col-1,21); //NW
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row-1,21); j = col; //N
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row-1,21); j = mod(col+1,21); //NE
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = mod(col-1,21); //W
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = col; //C
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = row; j = mod(col+1,21); //E
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row+1,21); j = mod(col-1,21); //SW
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row+1,21); j = col; //South
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;

    i = mod(row+1,21); j = mod(col+1,21); //South E
    neighbourhood.push_back(currentMap[i][j]);
    //cerr << "row:" << i << " col:" << j << " = " << currentMap[i][j] << endl;
  }

  return neighbourhood;
}

int CellularAutomata::mod(int x, int y) {
  int ret = x % y;
  if (ret < 0)
    ret += y;
  return ret;
}

int CellularAutomata::updateFunction(const vector<int> &cellNeighbours) {
  int result = 0;
  std::map<vector<int>,int>::iterator it;
  it = rulesMap.find(cellNeighbours);
  if(it != rulesMap.end()){
    result = rulesMap.find(cellNeighbours)->second;
    if(result == cellNeighbours[neighbourhoodSize/2]){
      return result;
    } else{
      changedStates++;
    }
  } else {
    result = cellNeighbours[neighbourhoodSize/2];
  }
  return result;
}

void CellularAutomata::swapMaps() {
  currentMap.swap(newMap);
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
  cerr << "Max number of rules = " << pow((double) states, (double) neighbourhoodSize) << endl;
  unsigned long long count = 0;
  ofstream fout(filename, std::ofstream::out);
  fout << neighbourhoodSize << endl;
  int result = 0;

  for (auto const& rule : rulesMap)
  {
    result = rule.second;
    if(result != rule.first[neighbourhoodSize/2]){
      for (int i = 0; i < rule.first.size(); i++) {
        fout << rule.first[i] << " ";
      }
      fout << result << endl;
      count++;
    }
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


