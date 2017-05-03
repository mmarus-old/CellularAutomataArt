//
// Created by archie on 4/17/17.
//

#include <iostream>
#include "Evolution.h"

using namespace std;

Evolution::Evolution() {

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

      bestEver = Chromosome(population[i]);
    }
  }
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

bool Evolution::isEnd() {
  if (currentGeneration < maxGenerations) {
    return false;
  } else {
    return true;
  }
}



void Evolution::exportChromosome() {
  bestEver.exportCA();
}

void Evolution::crossover() {
  vector<int> key1, key2;
  std::map<vector<int>,int>::iterator it;
  int lastIndexOfKeyNotInOldParent1 = -1;

  if (Chromosome::urandom(0, 101) < crossoverProbability){
    Chromosome oldParent1 = parent1;
    Chromosome oldParent2 = parent2;

    int crossoverCount = Chromosome::urandom(0, parent1.maxRules);
    for (int i = 0; i < crossoverCount; ++i) {
      key1 = oldParent1.rulesKeys[i];
      it = oldParent2.rulesMap.find(key1);
      if(it != oldParent2.rulesMap.end()){
        parent1.rulesMap[key1] = oldParent2.rulesMap[key1];
        parent2.rulesMap[key1] = oldParent1.rulesMap[key1];
      }
      else {
        //key1 not in oldParent2
        //find key2 which is not present in newParent1 ?
        for (int j = lastIndexOfKeyNotInOldParent1+1; j < oldParent2.rulesKeys.size(); ++j) {
          lastIndexOfKeyNotInOldParent1 = -1;
          key2 = oldParent2.rulesKeys[j];
          it = oldParent1.rulesMap.find(key2);
          if(it == oldParent1.rulesMap.end()){
            lastIndexOfKeyNotInOldParent1 = j;
            break;
          }
        }
        if(lastIndexOfKeyNotInOldParent1 != -1){
          parent1.rulesMap.erase(key1);
          parent1.rulesMap[key2] = oldParent2.rulesMap[key2];
          parent1.rulesKeys[i] = key2;

          parent2.rulesMap.erase(key2);
          parent2.rulesMap[key1] = oldParent1.rulesMap[key1];
          parent2.rulesKeys[lastIndexOfKeyNotInOldParent1] = key1;
        }

      }


    }
    parent1.evaluate = true;
    parent2.evaluate = true;
  }
}



