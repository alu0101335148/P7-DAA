/**
 * @file route.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief This file contains the definition of the class Route.
 * @version 1.2
 * @date 2022-04-14
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
     * @brief setter of the route
     * @param route vector of nodes
     */
    void setRoute(std::vector<int> route) {
      route_ = route;
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

    int& operator[](int pos) {
      return route_[pos];
    }

    void swap(int pos1, int pos2) {
      std::swap(route_[pos1], route_[pos2]);
    }

    void Displace(int first_index, int second_index) {
      if (first_index < second_index) {
        route_.insert(route_.begin() + second_index + 1, route_[first_index]);
        route_.erase(route_.begin() + first_index);
      }
      if (first_index > second_index) {
        int temp = route_[first_index];
        route_.erase(route_.begin() + first_index);
        route_.insert(route_.begin() + second_index + 1, temp);
      }
    }

    void insert(int index, int node) {
      route_.insert(route_.begin() + index + 1, node);
    }

    int remove(int index) {
      int node = route_[index];
      route_.erase(route_.begin() + index);
      return node;
    }
};

#endif