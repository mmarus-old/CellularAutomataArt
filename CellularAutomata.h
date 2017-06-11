//
// Created by archie on 4/17/17.
//

#include <fstream>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include "params.h"

#ifndef BIN_EVO_PROJEKT_CELLULARAUTOMATA_H
#define BIN_EVO_PROJEKT_CELLULARAUTOMATA_H

using namespace std;


class CellularAutomata {
public:
    CellularAutomata();
    int mathFunction(const vector<int> &cellNeighbours);
    int maxValueFromFunction = MAXRULES; // TODO: ZMENIT PRI ZMENE FUNKCIE
    vector<int> rulesVector;
    void initializeCA();
    void setFirstState();
    void exportCurrentState(string filename);
    int updateFunction();
    int updateFunction(const vector<int> &cellNeighbours);
    int valueOfcenterOfNeigh;
    int rowOfCenterOfNeigh;
    int colOfCenterOfNeigh;
    void develop();
    void runSimulation();
    void exportRules(string filename);
    void swapMaps();

    //MUST BE THE SAME
    int heigth = SIZEOFCA;
    int width = SIZEOFCA;
    vector< vector <int> > currentMap;
    vector<vector< vector <int> >> oldStates;
    bool isDead();
    int changedStates;
    int stepsWithChangedStates;
    bool isCrossedOver;
    vector<vector< vector <int> >>mapWithVisitedStates;

private:
    vector< vector <int> > newMap;
    ofstream* fout;
    int neighbourhoodSize = NEIGHBOURHOOD;
    int states = STATES;
    int simulationSteps = SIMULATIONSTEPS;
    int mathFunction();

};


#endif //BIN_EVO_PROJEKT_CELLULARAUTOMATA_H
