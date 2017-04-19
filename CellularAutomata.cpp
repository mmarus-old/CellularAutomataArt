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
    currentMap.resize(heigth, vector<int>(width));
    newMap.resize(heigth, vector<int>(width));
}

void CellularAutomata::setFirstState() {
    changedStates = 0;
    isCrossedOver = false;
    for (int i = 0; i < heigth; ++i) {
        for (int j = 0; j < width; ++j) {
            currentMap[i][j] = 0;
        }
    }

    currentMap[heigth/2][width/2] = 1;
    currentMap[heigth/2-1][width/2] = 1;
    currentMap[heigth/2][width/2-1] = 1;
    currentMap[heigth/2+1][width/2] = 1;
    currentMap[heigth/2][width/2+1] = 1;


//    currentMap[20][20] = 1;
//    currentMap[20][0] = 2;
//    currentMap[20][1] = 3;
//    currentMap[0][20] = 4;
//    currentMap[0][0] = 5;
//    currentMap[0][1] = 6;
//    currentMap[1][20] = 7;
//    currentMap[1][0] = 8;
//    currentMap[1][1] = 9;

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

void CellularAutomata::develop() {
    vector<int> cellNeighbours;

    for (int i = 0; i < heigth; ++i) {
        for (int j = 0; j < width; ++j) {
            cellNeighbours = getNeighbourhood(i,j);
            newMap[i][j] = updateFunction(cellNeighbours);
            if((i == width-1 || j == width-1 || i == 0 || j == 0) &&  newMap[i][j] != 0)
                isCrossedOver = true;
        }
    }
    swapMaps();
}

int CellularAutomata::updateFunction(const vector<int> &cellNeighbours) {
    int result = 0;
    std::map<vector<int>,int>::iterator it;
    it = rulesMap.find(cellNeighbours);
    if(it != rulesMap.end()){
        result = rulesMap.find(cellNeighbours)->second;
        if(result == cellNeighbours[neighbourhoodSize/2]){
            cout << "hmmm pravidlo co nic nerobi!" << endl;
            return result;
        } else{
            changedStates++;
//            cout << "changed states " << changedStates << endl;
        }
    } else {
        result = cellNeighbours[neighbourhoodSize/2];
    }
    return result;
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

void CellularAutomata::swapMaps() {
    currentMap.swap(newMap);
}

void CellularAutomata::runSimulation() {
    for (int i = 0; i < simulationSteps; ++i) {
        //isDead spomaluje albeo zrychluje ?
//        if(isDead() || crossedOver())
//            return;
        develop();
    }
}

void CellularAutomata::exportRules(string filename) {
    cout << "Max number of rules = " << pow((double) states, (double) neighbourhoodSize) << endl;
    unsigned long long count = 0;
    ofstream fout(filename, std::ofstream::out);
    fout << neighbourhoodSize << endl;
    std::vector< int > neighbourhood(neighbourhoodSize,0);
    int result = 0;
    do{
        result = updateFunction(neighbourhood);
        if(result != neighbourhood[neighbourhoodSize/2]){
            for (int i = 0; i < neighbourhood.size(); i++) {
                fout << neighbourhood[i] << " ";
            }
            fout << result << endl;
            count++;
        }

    }while( next_variation<int>( neighbourhood.begin(), neighbourhood.end(), states, 0) );

    cout << "# of rules = " << count << endl;
    fout.close();
}

void CellularAutomata::setRulesMap(map<vector<int>, int> rules) {
    rulesMap = rules;
}

bool CellularAutomata::isDead() {
    for (int i = 0; i < heigth; ++i) {
        for (int j = 0; j < width; ++j) {
            if(currentMap[i][j] != 0)
                return false;
        }
    }
    return true;
}

bool CellularAutomata::crossedOver() {
    //ak prekroci hranicu => fittnes = 0
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < width; ++j) {
            if(currentMap[i*(width-1)][j] != 0 || currentMap[j][i*(width-1)] !=0){
                return true;
            }
        }
    }
    return false;
}




