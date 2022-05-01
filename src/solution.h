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
#include <climits>

/**
 * @brief Class that stores the information about the solution
 */
class Solution {
  private:
    std::vector<Route> routes_ = {};
    int cost_ = INT_MAX;
  public:
  
    /** 
     * @brief Construct a new Solution object
     * @param num_vehicles number of vehicles (number of routes)
     */
    Solution(int num_vehicles) {
      routes_.resize(num_vehicles);
    }

    /**
     * @brief Construct a new Solution object
     * @param routes 
     */
    Solution(std::vector<Route> routes) {
      routes_ = routes;
      for (int i = 0; i < routes_.size(); i++) {
        cost_ += routes_[i].getCost();
      }
    }

    /** @brief Destroy the Solution:: Solution object */
    ~Solution() {};

    /**
     * @brief Method to add a route to the solution
     * @param route 
     */
    void addRoute(Route& route) {
      routes_.push_back(route);
    };

    /** @brief Function that recalculate the cost of the actual solution */
    int calculateCost() {
      cost_ = 0; 
      for (size_t i = 0; i < routes_.size(); i++) {
        cost_ += routes_[i].getCost();
      }
      return cost_;
    };

    /** @brief Function that returns the cost of the actual solution */
    int getCost() {
      return cost_;
    };

    /**
     * @brief Getter of the vector of routes
     * @return std::vector<Route> 
     */
    std::vector<Route>& getRoutes() {
      return routes_;
    };

    /** @brief Function that prints the solution */
    void printSolution() {
      int total_cost = 0;
      for (size_t i = 0; i < routes_.size(); i++) {
        std::cout << "Route " << i << ": ";
        routes_[i].printRoute();
        total_cost += routes_[i].getCost();
      }
      std::cout << "Total cost: " << total_cost << "\n";
    };

    /**
     * @brief Overload of the operator =
     * @param solution 
     */
    void operator=(Solution solution) {
      this->routes_ = solution.getRoutes();
      this->cost_ = solution.calculateCost();
    };
};

#endif
