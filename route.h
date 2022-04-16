/**
 * @file route.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief This file contains the definition of the class Route.
 * @version 1.2
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ___ROUTE___
#define ___ROUTE___

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief This class implement a basic route.
 * @details This class have a vector of nodes, that represents the route and
 * the cost of the route.
 */
class Route {
  private:
    std::vector<int> route_ = {};
    int cost_ = 0;

  public:
    /**
     * @brief Constructor of the class
     */
    Route() {};
    /**
     * @brief Destructor of the class
     */
    ~Route() {};

    /**
     * @brief Get the Size object
     * @return int size
     */
    int getSize() {
      return route_.size();
    }

    /**
     * @brief Get the Route object
     * @return std::vector<int> route
     */
    std::vector<int> getRoute() {
      return route_;
    }

    /**
     * @brief Get the Cost object
     * @return int& cost of the route
     */
    int& getCost() {
      return cost_;
    }

    /**
     * @brief Get the last node of the route
     * @return int las node
     */
    int getLastClient() {
      return route_.back();
    }

    /**
     * @brief This method adds a new node to the route
     * @param client node to add
     */
    void addClient(int client) {
      route_.push_back(client);
    }

    /**
     * @brief This method prints the route and the cost of the route
     */
    void printRoute() {
      std::string result = "[ ";
      for (int i = 0; i < route_.size(); i++) {
        result += std::to_string(route_[i]);
        if (i != route_.size() - 1) {
          result += " → ";
        }
      }
      std::cout << result << " ]\tCost: " << cost_ << "\n";
    }
};

#endif