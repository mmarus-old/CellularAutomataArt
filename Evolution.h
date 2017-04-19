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
    void exportChromosome();

private:
    Chromosome bestEver;
    vector<Chromosome> population;
    vector<Chromosome> nextPopulation;
    Chromosome parent1;
    Chromosome parent2;

    int mutationProbability = MUTATIONPROB;
    int mutatedGenes = MUTATEDGENES;
    int crossoverProbability = CROSSOVERPROBABILITY;
    int tournamentSize = TOURNAMENTSIZE; //Min 2
    int populationSize = POPSIZE; //Musi byt parne
    int maxGenerations = MAXGENERATIONS;
    int currentGeneration = 0;
    void createNewPopulation();
    void crossover();

};


#endif //BIN_EVO_PROJEKT_EVOLUTION_H
