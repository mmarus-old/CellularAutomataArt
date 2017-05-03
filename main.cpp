#include <iostream>
#include "Evolution.h"

using namespace std;

#define STRINGIFY(x) #x
#define STRINGIFYMACRO(y) STRINGIFY(y)

int main() {
    time_t seed = time(0);
    srand(seed);

    Evolution evolution;
    evolution.initialize();
    evolution.evolve();
    evolution.exportChromosome();

//    Chromosome c;
//    c.initialize();
//    c.calculateFittness();
//    c.exportCA();
//    cout << "fitness " << c.fittness << endl;

  string filename = "bicas/" + to_string(FILENAME) + "info";
  cout << filename << endl;
  ofstream fout(filename, std::ofstream::out);

  fout << "Seed"<< ",";
  fout << "MUTATIONPROB" << ",";
  fout << "MUTATEDGENES" << ",";
  fout << "CROSSOVERPROBABILITY" << ",";
  fout << "TOURNAMENTSIZE" << ",";
  fout << "POPSIZE" << ",";
  fout << "MAXGENERATIONS" << ",";
  fout << "STATES" << ",";
  fout << "NEIGHBOURHOOD" << ",";
  fout << "SIMULATIONSTEPS" << ",";
  fout << "SIZEOFCA" << ",";
  fout << "MINRULES" << ",";
  fout << "MAXRULES" << ",";
  fout << endl;

  fout << seed<< ",";
  fout << STRINGIFYMACRO(MUTATIONPROB) << ",";
  fout << STRINGIFYMACRO(MUTATEDGENES) << ",";
  fout << STRINGIFYMACRO(CROSSOVERPROBABILITY) << ",";
  fout << STRINGIFYMACRO(TOURNAMENTSIZE) << ",";
  fout << STRINGIFYMACRO(POPSIZE) << ",";
  fout << STRINGIFYMACRO(MAXGENERATIONS) << ",";
  fout << STRINGIFYMACRO(STATES) << ",";
  fout << STRINGIFYMACRO(NEIGHBOURHOOD) << ",";
  fout << STRINGIFYMACRO(SIMULATIONSTEPS) << ",";
  fout << STRINGIFYMACRO(SIZEOFCA) << ",";
  fout << STRINGIFYMACRO(MINRULES) << ",";
  fout << STRINGIFYMACRO(MAXRULES) << ",";
  fout << endl;

  fout.close();

  return 0;

}