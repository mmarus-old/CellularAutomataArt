//
// Created by archie on 4/17/17.
//

#import <vector>
#include <map>

#ifndef BIN_EVO_PROJEKT_CELLULARAUTOMATA_H
#define BIN_EVO_PROJEKT_CELLULARAUTOMATA_H

using namespace std;
#define STATES 2
#define NEIGHBOURHOOD 5;


class CellularAutomata {
public:
    CellularAutomata();
    void initializeCA();
    void setFirstState();
    void exportCurrentState(string filename);
    int updateFunction(const vector<int> &cellNeighbours);
    vector<int> getNeighbourhood(int row, int col);
    int mod(int x, int y);
    void develop();
    void runSimulation();
    void exportRules(string filename);
    void swapMaps();

    //MUST BE THE SAME
    int heigth = 31;
    int width = 31;
    vector< vector <int> > currentMap;
    map<vector<int>, int> rulesMap;

    void setRulesMap(map<vector<int>, int> rules);
    bool isDead();
    int changedStates;
    bool crossedOver();


    bool isCrossedOver;
private:
    vector< vector <int> > newMap;

    int neighbourhoodSize = NEIGHBOURHOOD;
    int states = STATES;
    int simulationSteps = 30;
    int usingUpdateRules = false;

};


#endif //BIN_EVO_PROJEKT_CELLULARAUTOMATA_H
