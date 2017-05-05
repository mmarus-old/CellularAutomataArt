//
// Created by archie on 4/17/17.
//

#ifndef BIN_EVO_PROJEKT_EVOLUTION_H
#define BIN_EVO_PROJEKT_EVOLUTION_H

#include "params.h"
#include "Chromosome.h"

class Evolution {
public:
    Evolution();
    void initialize();
    void evaluatePopulation();
    void evolve();
    bool isEnd();
    Chromosome mutate(Chromosome item, int percent);
    void exportChromosome(string suffix);

    Chromosome bestEver;
private:
    vector<Chromosome> population;
    vector<Chromosome> nextPopulation;
    Chromosome parent1;
    Chromosome parent2;

    int mutationProbability;
    int mutatedGenes;
    int crossoverProbability;
    int tournamentSize;
    int populationSize;
    int maxGenerations;
    int currentGeneration;
    void createNewPopulation();
    void crossover();

};


#endif //BIN_EVO_PROJEKT_EVOLUTION_H
