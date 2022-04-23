
#ifndef ___ALGORITHM_H___
#define ___ALGORITHM_H___

// Esta clase debe de almacenar la implementación de los algoritmos (greedy y GRASP)
#include <climits>

#include "problem.h"
#include "solution.h"

typedef std::pair<int, int> Pair;

class Algorithm {
  private:
    Problem* problem_;
  public:
    Algorithm(Problem* problem) {
      problem_ = problem;
    }
    ~Algorithm() {};
    Solution greedySolver(const int initialNode = 0);
    Solution GRASPSolver(const int initialNode = 0) {};
    Solution GRC(int seed, const int initialNode = 0);
  private:
    bool allClientsVisited(const std::vector<bool>& visited);
    Pair findMinNotVisited(const std::vector<bool>& visited,
                           const int& current);
    std::vector<float> calculateProbabilities(std::vector<int> avaibleClients,
                                              int actualNode);
    Pair findRandomMinNotVisited(std::vector<int> avaibleClients,
                                 int actualNode);

};

#endif
