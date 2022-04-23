/**
 * @file solution.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief Class that stores the vector of routes and the cost of the solution.
 * @version 1.0
 * @date 2022-04-19
 */

#ifndef ___SOLUTION_H___
#define ___SOLUTION_H___

#include "route.h"

/**
 * @brief Class that stores the information about the solution
 */
class Solution {
  private:
    std::vector<Route> routes_ = {};
    int cost_ = 0;
  public:
    Solution(int num_vehicles);
    ~Solution();

    void addRoute(Route& route);
    int calculateCost();

    std::vector<Route>& getRoutes();
    void printSolution();
};

/**
 * @brief Construct a new Solution:: Solution object
 */
Solution::Solution(int num_vehicles) {
  routes_.resize(num_vehicles);
}

/**
 * @brief Destroy the Solution:: Solution object
 */
Solution::~Solution() {}

/**
 * @brief Method to add a route to the solution
 * @param route 
 */
void Solution::addRoute(Route& route) {
  routes_.push_back(route);
}

/**
 * @brief Function that recalculate the cost of the actual solution
 */
int Solution::calculateCost() {
  cost_ = 0; 
  for (size_t i = 0; i < routes_.size(); i++) {
    cost_ += routes_[i].getCost();
  }
  return cost_;
}

/**
 * @brief Getter of the vector of routes
 * @return std::vector<Route> 
 */
std::vector<Route>& Solution::getRoutes() {
  return routes_;
}

void Solution::printSolution() {
  int total_cost = 0;
  for (size_t i = 0; i < routes_.size(); i++) {
    std::cout << "Route " << i << ": ";
    routes_[i].printRoute();
    total_cost += routes_[i].getCost();
  }
  std::cout << "Total cost: " << total_cost << "\n";
}

#endif
