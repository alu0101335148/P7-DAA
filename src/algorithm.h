/**
 * @file algorithm.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief File that contains the declaration of the algorithm class
 * @version 0.1
 * @date 2022-04-23
 */

#ifndef ___ALGORITHM_H___
#define ___ALGORITHM_H___

// Esta clase debe de almacenar la implementación de los algoritmos (greedy y GRASP)
#include <climits>

#include "problem.h"
#include "solution.h"

typedef std::pair<int, int> Pair;

/**
 * @brief Class algorith that implements the GRASP algorithm and the greedy
 * algorithm
 */
class Algorithm {
  public:
    /**
     * @brief Construct a new Algorithm object
     * @param problem 
     */
    Algorithm(Problem* problem) {
      problem_ = problem;
    }
    /** @brief Destroy the Algorithm object */
    ~Algorithm() {};

    Solution greedySolver(const int initialNode = 0);
    Solution GRASPSolver(const int initialNode = 0) {};
    Solution GRC(int seed, const int initialNode = 0);
  private:
    Problem* problem_;

    bool allClientsVisited(const std::vector<bool>& visited);
    Pair findMinNotVisited(const std::vector<bool>& visited,
                           const int& current);
    Pair findRandomMinNotVisited(std::vector<int> avaibleClients,
                                 int actualNode, int candidates = 2);

};

#endif
