//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Evolution.h"

using namespace std;

Evolution::Evolution() {
  mutationProbability = MUTATIONPROB;
  mutatedGenes = MUTATEDGENES;
  crossoverProbability = CROSSOVERPROBABILITY;
  tournamentSize = TOURNAMENTSIZE; //Min 2
  populationSize = POPSIZE; //Musi byt parne
  maxGenerations = MAXGENERATIONS;
  currentGeneration = 0;
}

void Evolution::initialize() {
  cerr << "MAX RULES = " << MAXRULES;
  cerr << endl;
  population.resize(populationSize);
  nextPopulation.resize(populationSize);

  for (int i = 0; i < populationSize; ++i) {
    population[i].initialize();
    population[i].fittness = 0;
    nextPopulation[i].initialize();
    nextPopulation[i].fittness = 0;
  }

  bestEver = population[0];
}

void Evolution::evolve() {
  while (!isEnd()) {
    evaluatePopulation();
    nextPopulation[0] = bestEver;
    nextPopulation[1] = mutate(bestEver, 100);
    createNewPopulation();

    swap(population, nextPopulation);

    cerr << "Generation: " << currentGeneration << " max fitness = " << bestEver.fittness << endl;
    currentGeneration++;
  }
  cout << bestEver.fittness << ";";
}

bool Evolution::isEnd() {
  if(bestEver.fittness >= MAXFITENSS)
    return true;
  if (currentGeneration < maxGenerations) {
    return false;
  } else {
    return true;
  }
}

void Evolution::evaluatePopulation() {
  for (int i = 0; i < populationSize; ++i) {
    population[i].calculateFittness();
  }

  for(int i = 0; i < populationSize; i++) {
    if (population[i].getFittness() > 0 && population[i].getFittness() >= bestEver.getFittness()) {
      //TODO: MOZNO TREBA SKOPIROVAT NEJAKO
      if (population[i].getFittness() > bestEver.getFittness()) {
        cerr << "Generacia #" << currentGeneration << endl;
        cerr << "Best fittnes = " << bestEver.fittness << endl;
      }
//      exportChromosome(to_string(currentGeneration)+"-"+to_string(i));
      bestEver = Chromosome(population[i]);
    }
  }
}

void Evolution::exportChromosome(string suffix) {
  bestEver.exportCA(suffix);
}

Chromosome Evolution::mutate(Chromosome item, int percent) {
  Chromosome mutant = Chromosome(item);

  if (Chromosome::urandom(0, 101) <= percent) {
    for (int i = 0; i < mutatedGenes; ++i) {
      mutant.mutateRandomRule();
    }
    mutant.evaluate = true;
  }
  return mutant;
}

void Evolution::createNewPopulation() {
  for (int i = 2; i < populationSize; i += 2) {
    parent1 = population[Chromosome::urandom(0, populationSize)];
    parent2 = population[Chromosome::urandom(0, populationSize)];
    for (int t = 2; t < tournamentSize; t++) {
      Chromosome parent3 = population[Chromosome::urandom(0, populationSize)];
      if (parent3.getFittness() > parent1.getFittness())
        parent1 = parent3;
      else if (parent3.getFittness() > parent2.getFittness()) {
        parent2 = parent3;
      }
    }
    crossover();
    parent1 = mutate(parent1, mutationProbability);
    parent2 = mutate(parent2, mutationProbability);
    nextPopulation[i] = parent1;
    nextPopulation[i + 1] = parent2;
  }
}

void Evolution::crossover() {
  if (Chromosome::urandom(0, 101) < crossoverProbability) {
    Chromosome oldParent1 = parent1;
    Chromosome oldParent2 = parent2;

    int crossoverPoint = Chromosome::urandom(0, parent1.ca.rulesVector.size());
    for (int i = crossoverPoint; i < parent1.ca.rulesVector.size(); ++i) {
      parent1.ca.rulesVector[i] = oldParent2.ca.rulesVector[i];
      parent2.ca.rulesVector[i] = oldParent1.ca.rulesVector[i];
    }

    parent1.evaluate = true;
    parent2.evaluate = true;
  }
}


