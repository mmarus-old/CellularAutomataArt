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
    map<vector<int>, int> rulesMap;
    int maxValueFromFunction = MAXRULES; // TODO: ZMENIT PRI ZMENE FUNKCIE
    vector<int> rulesVector;
    void initializeCA();
    void setFirstState();
    void exportCurrentState(string filename);
    int updateFunction(const vector<int> &cellNeighbours);
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
    int neighbourhoodSize = NEIGHBOURHOOD;
    int states = STATES;
    int simulationSteps = SIMULATIONSTEPS;

    vector<int> getNeighbourhood(int row, int col);

    int mod(int x, int y);
};


#endif //BIN_EVO_PROJEKT_CELLULARAUTOMATA_H
