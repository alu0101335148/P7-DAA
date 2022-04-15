#ifndef ___ROUTE___
#define ___ROUTE___

#include <iostream>
#include <string>
#include <vector>


class Route {
  private:
    std::vector<int> route_ = {};
    int cost_ = 0;

  public:
    Route() {};
    ~Route() {};

    int getSize() {
      return route_.size();
    }

    std::vector<int> getRoute() {
      return route_;
    }

    int& getCost() {
      return cost_;
    }

    int getLastClient() {
      return route_.back();
    }

    void addClient(int client) {
      route_.push_back(client);
    }

    void printRoute() {
      std::string result = "[ ";
      for (int i = 0; i < route_.size(); i++) {
        result += std::to_string(route_[i]);
        if (i != route_.size() - 1) {
          result += " → ";
        }
      }
      std::cout << result << " ] Cost: " << cost_ << "\n";
    }

};

#endif