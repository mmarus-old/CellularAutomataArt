//
// Created by archie on 4/17/17.
//

#ifndef BIN_EVO_PROJEKT_EVOLUTION_H
#define BIN_EVO_PROJEKT_EVOLUTION_H


#include "Chromosome.h"

class Evolution {
public:
    Evolution();
    void initialize();
    void evaluatePopulation();
    void evolve();
    bool isEnd();
    Chromosome mutate(Chromosome item, int percent);

    void exportChromosome();

private:
    Chromosome bestEver;
    vector<Chromosome> population;
    vector<Chromosome> nextPopulation;
    Chromosome parent1;
    Chromosome parent2;

    int mutationProbability = 4;
    int mutatedGenes = 4;
    int crossoverProbability = 70;
    int tournamentSize = 4; //Min 2
    int populationSize = 20; //Musi byt parne
    int maxGenerations = 20;
    int currentGeneration = 0;
    int chromosomeLength = 10;


    void createNewPopulation();

    void crossover();

};


#endif //BIN_EVO_PROJEKT_EVOLUTION_H
