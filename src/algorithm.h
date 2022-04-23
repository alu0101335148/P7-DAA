// Esta clase debe de almacenar la implementación de los algoritmos (greedy y GRASP)
#include "problem.h"
#include "solution.h"

class Algorithm {
  private:
    Problem* problem_;
    Solution solution_;
  public:
    Algorithm(Problem* problem) {
      problem_ = problem;
    }
    ~Algorithm() {};
    Solution greedySolver(Problem& problem);
    Solution GRASPSolver(Problem& problem);
};