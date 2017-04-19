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
    currentMap.resize(heigth, vector<int>(width));
    newMap.resize(heigth, vector<int>(width));
    oldStates.resize(SIMULATIONSTEPS, newMap);
}

int CellularAutomata::updateFunction() {
    int result = 0;
    int index = mathFunction();

    if(index < rulesVector.size()){
        result = rulesVector[index];
        if(result != valueOfcenterOfNeigh)
            changedStates++;
    } else {
        result = valueOfcenterOfNeigh;
    }

    return result;
}

int CellularAutomata::mathFunction() {
    int result = 0;

    if(neighbourhoodSize == 5) {
        result += currentMap[rowOfCenterOfNeigh-1][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh-1];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh+1][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh+1];
    } else if(neighbourhoodSize == 9) {
        result += currentMap[rowOfCenterOfNeigh-1][colOfCenterOfNeigh-1];
        result += currentMap[rowOfCenterOfNeigh-1][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh-1][colOfCenterOfNeigh+1];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh-1];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh][colOfCenterOfNeigh+1];
        result += currentMap[rowOfCenterOfNeigh+1][colOfCenterOfNeigh-1];
        result += currentMap[rowOfCenterOfNeigh+1][colOfCenterOfNeigh];
        result += currentMap[rowOfCenterOfNeigh+1][colOfCenterOfNeigh+1];
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

    if(index < rulesVector.size()){
        result = rulesVector[index];
        if(result != cellNeighbours[neighbourhoodSize/2])
            changedStates++;
    } else {
        result = cellNeighbours[neighbourhoodSize/2];
    }

    return result;
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

    for (int i = 1; i < heigth-1; ++i) {
        for (int j = 1; j < width-1; ++j) {
            if(i == width-1 || j == width-1 || i == 0 || j == 0){
                return;
            }
            valueOfcenterOfNeigh = currentMap[i][j];
            rowOfCenterOfNeigh = i;
            colOfCenterOfNeigh = j;
            newMap[i][j] = updateFunction();
            if((i == width -2 || j == width-2 || i == 1 || j == 1) && newMap[i][j] != 0 ) {
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

void CellularAutomata::swapMaps() {
    currentMap.swap(newMap);
}

void CellularAutomata::runSimulation() {
    int oldChangedStates;
    stepsWithChangedStates = 0;
    for (int i = 0; i < simulationSteps; ++i) {
        oldChangedStates = changedStates;
        develop();
        if(oldChangedStates == changedStates)
            break;
        else
            stepsWithChangedStates++;
        if(isCrossedOver)
            break;
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

void CellularAutomata::exportRules2(string filename) {
    countOfRulesWritten = 0;
    cout << "Max number of rules = " << pow((double) states, (double) neighbourhoodSize) << endl;
    ofstream fileOut(filename, std::ofstream::out);
    fout = &fileOut;
    *fout << neighbourhoodSize << endl;
    std::vector< int > neighbourhood(neighbourhoodSize,0);

    recursiveWriting(0, neighbourhood);


    cout << "# of rules = " << countOfRulesWritten << endl;

    (*fout).close();
}

void CellularAutomata::recursiveWriting(int index, vector<int> neighboursVector) {

    for (int i = 0; i < STATES; ++i) {
        neighboursVector[index] = i;
        int indexOfResult = mathFunction(neighboursVector);

        if(indexOfResult < MAXRULES){
            int result = rulesVector[indexOfResult];

            if(result != neighboursVector[neighbourhoodSize/2]){
                for (int i = 0; i < neighboursVector.size(); i++) {
                    (*fout)  << neighboursVector[i] << " ";
                }
                (*fout)  << result << endl;
                countOfRulesWritten++;
            }
        } else if (index >= neighbourhoodSize || indexOfResult > MAXRULES){
            return;
        }
        recursiveWriting(index+1, neighboursVector);
    }

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






